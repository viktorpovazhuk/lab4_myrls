// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
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

int EXIT_CODE = 0;

enum Errors {
    EDIR = 25,
    EFILE = 26,
    EWRPATH = 27,

};

struct both_slashes {
    bool operator()(char a, char b) const {
        return a == '/' && b == '/';
    }
};

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
    file_info info;
    char buffer [100];

    if (lstat(path.c_str(), &st) == -1){
        std::cerr << "Error accessing " << path << std::endl;
        EXIT_CODE = EFILE;
        info.err = true;
        return info;
    }



    info.path = path;

    info.permissions = get_permissions(st.st_mode);
    info.owner = get_owner(st.st_uid);
    info.size = st.st_size;

    std::string spec_symb;
    if (S_ISDIR(st.st_mode))
        spec_symb = "/";
    else if (S_ISLNK(st.st_mode))
        spec_symb = "@";
    else if ((st.st_mode & S_IEXEC) != 0)
        spec_symb = "*";
    else if (S_ISFIFO(st.st_mode))
        spec_symb = "|";
    else if (S_ISSOCK(st.st_mode))
        spec_symb = "=";
    else if (S_ISREG(st.st_mode))
        spec_symb = "";
    else
        spec_symb = "?";
    info.filename = spec_symb + std::string(basename(strdup(path.c_str())));

    if (spec_symb == "@") {
        char symlink_path[PATH_MAX];
        ssize_t path_len = readlink(path.c_str(), symlink_path, PATH_MAX);
        symlink_path[path_len] = '\0';
        info.filename += std::string(" -> ") + symlink_path;
        printf("Synlink_path = %s\n", symlink_path);
    }

    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));
    info.modification_time = buffer;

    return info;

}

char switch_case(char c) {
    if (isupper(c)) {
        return tolower(c);
    }
    return toupper(c);
}

int custom_comparator(const struct dirent **d1, const struct dirent **d2) {
    const char *name1 = (*d1)->d_name;
    const char *name2 = (*d2)->d_name;

    while (*name1 != '\0' && *name2 != '\0') {
        char switched1 = switch_case(*name1);
        char switched2 = switch_case(*name2);
        if (switched1 < switched2) {
            return -1;
        }
        if (switched1 > switched2) {
            return 1;
        }
        ++name1, ++name2;
    }

    if (*name2) {
        return -1;
    }

    return *name1;
}

int main(int argc, char* argv[]) {
    command_line_options_t command_line_options{argc, argv};
    if (argc > 2) {
        std::cerr << "Too much parameters" << std::endl;
        return EXIT_FAILURE;
    }
    std::string parent_dir;
    if (argc == 1)
        parent_dir = "./";
    else
        parent_dir = command_line_options.get_pathname();

    struct stat st;
    if (stat(parent_dir.c_str(), &st) == -1){
        std::cerr << "Error accessing initial path " << parent_dir << std::endl;
        EXIT_CODE = EWRPATH;
        return EXIT_CODE;
    }
    if (!S_ISDIR(st.st_mode)) {
        file_info info = get_file_info(parent_dir);

        std::cout << info.permissions << " " << info.owner << " " << info.size << " " << info.modification_time << " " << info.filename << std::endl;
        return 0;

    }

    std::stack<std::string> dirs_stack;
    dirs_stack.push(parent_dir);


    while (!dirs_stack.empty()) {
        struct dirent **entries;
        int num_entries;

        std::string cur_dir = dirs_stack.top();
        dirs_stack.pop();
        cur_dir.erase(std::unique(cur_dir.begin(), cur_dir.end(), both_slashes()), cur_dir.end());

        std::cout << cur_dir + ":" << '\n';

        num_entries = scandir(cur_dir.c_str(), &entries, nullptr, custom_comparator);

        if (num_entries < 0) {
            std::cerr << "Error opening " << cur_dir << std::endl;
            EXIT_CODE = EDIR;
            continue;
        }

        std::vector<std::string> subdirs;

        std::vector<file_info> dir_info;
        int  max_file_size = 0;
        for (int i = 0; i < num_entries; i++) {
            if (!strcmp(entries[i]->d_name, ".") or !strcmp(entries[i]->d_name, "..")) {
                continue;
            }

            struct dirent *entry = entries[i];
            std::string path = cur_dir + "/" + std::string(entry->d_name);

            file_info info = get_file_info(path);
            if (info.err)
                continue;
            dir_info.push_back(info);
            max_file_size = fmax( std::to_string(info.size).size(), max_file_size);



            if (entries[i]->d_type == DT_DIR) {
                subdirs.emplace_back(cur_dir + "/" + std::string(entries[i]->d_name));
            }


            free(entries[i]);
        }
        for (auto & info : dir_info) {
            std::cout << info.permissions << " " << info.owner << " " << std::string(max_file_size -  std::to_string(info.size).size(), ' ') << info.size << " " << info.modification_time << " " << info.filename << std::endl;
        }
        free(entries);

        for (int i = subdirs.size() - 1; i >= 0; i--) {
            dirs_stack.push(subdirs[i]);
        }

        std::cout << '\n';
    }

    return EXIT_CODE;
}
