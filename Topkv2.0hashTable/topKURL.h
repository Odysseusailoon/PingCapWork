//
//  topKURL.hpp
//  topKURL

#ifndef topKURL_h
#define topKURL_h

#include <vector>
#include <string>

class TopkURLs {
public:
    TopkURLs(const char *name, uint64_t sz, int n);
    int split();
    int sortSubFile();
    int selectTopK();
    int topK();
    int scanfDir(std::vector<std::string>& files);
    
private:
    std::string fileName;
    uint64_t memSZ;
    int k;
};

#endif /* topKURL_h */
