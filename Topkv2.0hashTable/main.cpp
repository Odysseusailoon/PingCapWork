//
//  main.cpp


#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>

#include "topKURL.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <urls filename> <memory size by byte> <top k>" << std::endl;
        return 1;
    }
    
    std::ifstream ifs(argv[1], std::ifstream::ate);
    if (!ifs) {
        std::cerr << "open " << argv[1] << " failed: " << strerror(errno) << "\n";
        return 1;
    }
    TopkURLs urls(argv[1], std::stoul(argv[2]), std::stol(argv[3]));
    int ret = urls.topK();
    if (ret != 0) {
        std::cerr << "get top k URLs count failed: " << std::strerror(errno) << std::endl;
        return errno;
    }
    
    return 0;
}
