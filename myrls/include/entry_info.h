//
// Created by vivi on 29.10.22.
//

#ifndef MYRLS_FILE_INFO_H
#define MYRLS_FILE_INFO_H

#include <string>
#include <cstddef>

struct file_info {
    std::string path;

    std::string permissions;
    std::string owner;
    std::size_t size;
    std::string modification_date;
    std::string modification_time;
    std::string filename;
    std::string type;
};

#endif //MYRLS_FILE_INFO_H
