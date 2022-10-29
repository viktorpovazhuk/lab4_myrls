//
// Created by vivi on 29.10.22.
//

#include "TreeEntry.h"

#include <memory>
#include <string>

TreeEntry::TreeEntry(std::string path, struct stat *file_info) {
    this->path = path;

}

