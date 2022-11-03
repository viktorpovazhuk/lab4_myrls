#include <dirent.h>
#include <cstdio>
#include <string>

int main() {

    dirent **dirent_list;

    int num_entries = scandir(".", &dirent_list, nullptr, versionsort);

    if (num_entries == -1) {
        perror("scandir");
    }
//drwxrwxr-x 3 bohdan bohdan 4096 лис  3 13:00 myrls
//-rw-rw-r-- 1 bohdan bohdan 25441 лис  3 13:00 CMakeCXXCompilerId.cpp
    for (int no_entry = 0; no_entry < num_entries; ++no_entry) {
        dirent *dirent = dirent_list[no_entry];
        printf("%s\n", dirent->d_name);
    }

    return 0;
}