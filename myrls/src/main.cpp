// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <dirent.h>
#include <stack>

#include "options_parser.h"
#include "file_info.h"

file_info get_file_info(const std::string &path, const struct stat *stat) {
    file_info info;

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

            if (entries[i]->d_type == DT_DIR) {
                subdirs.emplace_back(cur_dir + "/" + std::string(entries[i]->d_name));
            }

            std::cout << entries[i]->d_name << '\n';
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
