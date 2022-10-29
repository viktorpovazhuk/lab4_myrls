//
// Created by vivi on 29.10.22.
//

#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#ifndef MYRLS_TREEENTRY_H
#define MYRLS_TREEENTRY_H

class TreeEntry {
public:
    TreeEntry();
    TreeEntry(std::string path, struct stat *file_info);

    std::vector<std::unique_ptr<TreeEntry>> get_children();
    void add_child(TreeEntry &treeEntry);

    void set_parent(TreeEntry &treeEntry);
private:
    std::string path;

    std::string rights;
    std::string owner;
    size_t size;
    std::string modification_date;
    std::string modification_time;
    std::string filename;
    std::string type;

    std::vector<std::unique_ptr<TreeEntry>> children;
    std::unique_ptr<TreeEntry> parent;
};


#endif //MYRLS_TREEENTRY_H
