#include <dirent.h>
#include <cstdio>
#include <string>

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

int main() {

    dirent **dirent_list;

    int num_entries = scandir(".", &dirent_list, nullptr, custom_comparator);

    if (num_entries == -1) {
        perror("scandir");
    }

    for (int no_entry = 0; no_entry < num_entries; ++no_entry) {
        dirent *dirent = dirent_list[no_entry];
        printf("%s\n", dirent->d_name);
    }

    return 0;
}