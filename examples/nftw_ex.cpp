//
// Created by vivi on 28.10.22.
//

#include <ftw.h>
#include <iostream>
#include <utility>
#include <boost/property_tree/ptree.hpp>


class dir_entry {
public:
    int fd = 10;
    std::string path = "/";
};

typedef boost::property_tree::basic_ptree<std::string, dir_entry> ptree;

int main()
{
    dir_entry e;
    e.path = "/";
    ptree pt;
    pt.put(ptree::path_type{"C:\\Windows\\System", '\\'}, e);

    ptree &windows = pt.get_child(ptree::path_type{"C:\\Windows", '\\'});
    std::string files;
    for (const std::pair<std::string, ptree> &p : windows)
        files += p.second.get_value<dir_entry>().path;
    std::cout << files << '\n';
}

//static int display_info(const char *fpath, const struct stat *sb,
//                        int tflag, struct FTW *ftwbuf) {
//    printf("%-3s %2d %7jd   %-40s %d %s\n",
//           (tflag == FTW_D) ? "d" : (tflag == FTW_DNR) ? "dnr" :
//                                    (tflag == FTW_DP) ? "dp" : (tflag == FTW_F) ? "f" :
//                                                               (tflag == FTW_NS) ? "ns" : (tflag == FTW_SL) ? "sl" :
//                                                                                          (tflag == FTW_SLN) ? "sln"
//                                                                                                             : "???",
//           ftwbuf->level, (intmax_t) sb->st_size,
//           fpath, ftwbuf->base, fpath + ftwbuf->base);
//    return 0;           /* To tell nftw() to continue */
//}
//
//int main() {
//    int flags = 0;
//
//    flags |= FTW_PHYS;
//
//    if (nftw("/home/vivi/Downloads/temp", display_info, 20, flags)
//        == -1) {
//        perror("nftw");
//        exit(EXIT_FAILURE);
//    }
//
//    exit(EXIT_SUCCESS);
//}