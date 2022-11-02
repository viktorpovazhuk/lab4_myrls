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

#include "options_parser.h"
#include "file_info.h"

file_info get_file_info(const std::string &path, const struct stat *stat) {
    file_info info;

}

std::string get_permissions(const mode_t &st_permissions) {
    std::string permissions;
    permissions += (st_permissions & S_IRUSR) ? 'r' : '-';
    permissions += (st_permissions & S_IWUSR) ? 'r' : '-';
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

int main(int argc, char* argv[]) {
    std::string parent_dir = "/home/vivi/Downloads/temp";

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
            struct stat entry_stat{};
            stat(path.c_str(), &entry_stat);

            file_info info;
            info.path = path;
            info.filename = entry->d_name;
            info.permissions = get_permissions(entry_stat.st_mode);
            info.owner = get_owner(entry_stat.st_uid);

            if (entries[i]->d_type == DT_DIR) {
                subdirs.emplace_back(cur_dir + "/" + std::string(entries[i]->d_name));
            }

            printf("%s %10s %15s\n", info.permissions.c_str(), info.owner.c_str(), info.filename.c_str());

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
