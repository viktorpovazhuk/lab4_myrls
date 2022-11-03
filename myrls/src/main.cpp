// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <string>
#include <dirent.h>
#include <stack>
#include <libgen.h>
#include <filesystem>
#include <time.h>
#include <stdio.h>



#include "options_parser.h"
#include "entry_info.h"


std::string get_permissions(const mode_t &st_permissions) {
    std::string permissions;
    permissions += (st_permissions & S_IRUSR) ? 'r' : '-';
    permissions += (st_permissions & S_IWUSR) ? 'w' : '-';
    permissions += (st_permissions & S_IXUSR) ? 'x' : '-';
    permissions += (st_permissions & S_IRGRP) ? 'r' : '-';
    permissions += (st_permissions & S_IWGRP) ? 'w' : '-';
    permissions += (st_permissions & S_IXGRP) ? 'x' : '-';
    permissions += (st_permissions & S_IROTH) ? 'r' : '-';
    permissions += (st_permissions & S_IWOTH) ? 'w' : '-';
    permissions += (st_permissions & S_IXOTH) ? 'x' : '-';

    return permissions;
}

std::string get_owner(const uid_t &uid) {
    struct passwd *pw;
    std::string owner;
    pw = getpwuid(uid);
    if (pw) {
        owner = pw->pw_name;
    }
    else {
        owner = std::to_string(uid);
    }

    return owner;
}

file_info get_file_info(const std::string &path) {
    struct stat st;
    char buffer [100];
    stat(path.c_str(), &st);

    file_info info;
    info.path = path;

    info.permissions = get_permissions(st.st_mode);
    info.owner = get_owner(st.st_uid);
    info.size = st.st_size;

    std::string spec_symb;
    if (S_ISDIR(st.st_mode))
        spec_symb = "/";
    else if ((st.st_mode & S_IEXEC) != 0)
        spec_symb = "*";
    else if (S_ISFIFO(st.st_mode))
        spec_symb = "|";
    else if (S_ISLNK(st.st_mode))
        spec_symb = "@";
    else if (S_ISSOCK(st.st_mode))
        spec_symb = "=";
    else if (S_ISREG(st.st_mode))
        spec_symb = "";
    else
        spec_symb = "?";
    info.filename = spec_symb + std::string(std::filesystem::path(path).filename());

    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));
    info.modification_time = buffer;

    return info;

}

int main(int argc, char* argv[]) {
    command_line_options_t command_line_options{argc, argv};
    if (argc > 3) {
        std::cerr << "Too much parameters" << std::endl;
        exit(1);
    }
    std::string parent_dir;
    if (argc == 1)
        parent_dir = ".";
    else
        parent_dir = command_line_options.get_pathname();

    if (!std::filesystem::is_directory(parent_dir)) {


        file_info info = get_file_info(parent_dir);

        printf("%s %10s %15s %lu %s\n", info.permissions.c_str(), info.owner.c_str(), info.filename.c_str(), info.size, info.modification_time.c_str());
        exit(0);
    }
    std::cout << parent_dir << std::endl;


    std::stack<std::string> dirs_stack;
    dirs_stack.push(parent_dir);

    while (!dirs_stack.empty()) {
        struct dirent **entries;
        int num_entries;

        std::string cur_dir = dirs_stack.top();
        dirs_stack.pop();

        std::cout << cur_dir + ":" << '\n';

        num_entries = scandir(cur_dir.c_str(), &entries, nullptr, alphasort);

        std::vector<std::string> subdirs;
        for (int i = 0; i < num_entries; i++) {
            if (!strcmp(entries[i]->d_name, ".") or !strcmp(entries[i]->d_name, "..")) {
                continue;
            }

            struct dirent *entry = entries[i];
            std::string path = cur_dir + "/" + std::string(entry->d_name);


            file_info info = get_file_info(path);

            if (entries[i]->d_type == DT_DIR) {
                subdirs.emplace_back(cur_dir + "/" + std::string(entries[i]->d_name));
            }

            printf("%s %10s %15s %lu %s\n", info.permissions.c_str(), info.owner.c_str(), info.filename.c_str(), info.size, info.modification_time.c_str());

            free(entries[i]);
        }
        free(entries);

        for (int i = subdirs.size() - 1; i >= 0; i--) {
            dirs_stack.push(subdirs[i]);
        }

        std::cout << '\n';
    }

    return 0;
}
