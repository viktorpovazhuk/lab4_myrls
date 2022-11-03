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
    bool err = false;
};

#endif //MYRLS_FILE_INFO_H
