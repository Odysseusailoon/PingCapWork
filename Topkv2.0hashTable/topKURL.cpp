//
//  topKURL.cpp
//  topKURL
//

#include "topKURL.h"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>


TopkURLs::TopkURLs(const char *name, uint64_t size, int _k) :
fileName(name), memSZ(size), k(_k) {}

int TopkURLs::scanfDir(std::vector<std::string>& files) {
    DIR *dirp = opendir("./data");
    if(dirp == NULL) {
        std::cerr << "open directory \"data\" failed: " << std::strerror(errno) << std::endl;
        return errno;
    }
    
    struct dirent *dent;
    while ((dent = readdir(dirp)) != NULL) {
        if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) continue;
        files.push_back(std::string("./data/") + dent->d_name);
    }
    closedir(dirp);
    return 0;
}

int TopkURLs::split() {
    struct stat st;
    if(stat(fileName.c_str(), &st) != 0) {
        std::cerr << "stat " << fileName << " failed: " << std::strerror(errno) << std::endl;
        return errno;
    }
    
    uint64_t length = st.st_size;
    int num = (int)length / memSZ * 2;
    
    if(num == 0) num = 1;
    
    // then create data directory
    if(stat("./data", &st) != 0) {
        if(errno == ENOENT) {
            int ret = mkdir("./data", 0775);
            if(ret != 0) {
                std::cerr << "mkdir \"data\" directory failed " << std::strerror(errno) << std::endl;
                return errno;
            }
        } else {
            std::cerr << "open \"data\" directory failed " << std::strerror(errno) << std::endl;
            return errno;
        }
    } else {
        if(!S_ISDIR(st.st_mode)) {
            std::cerr << "already exist \"data\", please deleted it first " << std::strerror(errno) << std::endl;
            return errno;
        }
    }
    
    std::vector<std::string> files;
    int ret = scanfDir(files);
    if(ret != 0) return ret;
    
    for(auto fp : files) {
        if(remove(fp.c_str()) != 0) {
            std::cerr << "remove file " << fp << " failed: " << std::strerror(errno) << std::endl;
            return errno;
        }
    }
    
    // hash to split file
    std::string line;
    std::hash<std::string> strHash;
    std::ifstream ifs(fileName);
    while (std::getline(ifs, line)) {
        size_t slot = strHash(line) % num;
        std::string name = "./data/url" + std::to_string(slot) + ".dat";
        std::ofstream log(name, std::ofstream::app | std::ofstream::out );
        log << line << std::endl;
    }
    ifs.close();
    return 0;
}

// find topK
struct cmp {
    template <typename T>
    bool operator() (const T& lhs, const T& rhs) const {
        if(lhs.second != rhs.second) return lhs.second > rhs.second;
        return lhs.first > rhs.first;
    }
};

int TopkURLs::sortSubFile() {
    std::vector<std::string> files;
    int ret = scanfDir(files);
    if(ret != 0) return ret;
    
    for(auto fp : files) {
        std::ifstream ifs(fp);
        if(!ifs) continue;
        
        std::map<std::string, int> urls;
        std::string line;
        
        while (std::getline(ifs, line)) {
            urls[line]++;
        }
        ifs.close();
        
        std::set<std::pair<std::string, int>, cmp> cntURLs(urls.begin(), urls.end());
        
        std::ofstream ofs(fp, std::ofstream::trunc | std::ofstream::out);
        for(auto& pr : cntURLs) {
            ofs << pr.first << " " << pr.second << std::endl;
        }
        ofs.close();
    }
    return 0;
}

bool urlCmp(std::pair<std::string, int> lhs, std::pair<std::string, int> rhs) {
    return lhs.second < rhs.second;
}

int TopkURLs::selectTopK() {
    std::vector<std::ifstream* > fileList;
    std::vector<std::pair<std::string, int> > records;
    std::vector<std::string> files;
    
    int ret = scanfDir(files);
    if(ret != 0) return ret;
    
    for(auto fp : files) {
        std::ifstream *ifs = new std::ifstream(fp);
        fileList.push_back(ifs);
        std::string line;
        if(std::getline(*ifs, line)) {
            std::stringstream ss(line);
            std::string url;
            int cnt;
            ss >> url >> cnt;
            records.push_back(std::make_pair(url, cnt));
        } else {
            records.push_back(std::make_pair("", -1));
        }
    }
    
    std::ofstream ofs("topkurls.result", std::ofstream::trunc | std::ofstream::out);
    std::pair<std::string, int> maxans = *max_element(records.begin(), records.end(), urlCmp);
    int cnt = 0;
    while(maxans.second != -1) {
        cnt++;
        if(cnt > k) break;
        
        for(size_t i = 0; i < records.size(); i++) {
            if(maxans.second == records[i].second) {
                std::string line;
                for( ; ; ) {
                    ofs << records[i].first << " " << records[i].second << std::endl;
                    if(std::getline(*fileList[i], line)) {
                        std::stringstream ss(line);
                        std::string url;
                        int ct;
                        ss >> url >> ct;
                        records[i] = std::make_pair(url, ct);
                    } else {
                        records[i] = std::make_pair("", -1);
                        break;
                    }
                    
                    if(maxans.second != records[i].second) break;
                }
            }
        }
        
        maxans = *max_element(records.begin(), records.end(), urlCmp);
    }
    
    for(auto pfs : fileList) delete pfs;
    return 0;
}

int TopkURLs::topK() {
    int ret;
    ret = split();
    if(ret != 0) return ret;
    
    ret = sortSubFile();
    if(ret != 0) return ret;
    
    ret = selectTopK();
    if(ret != 0) return ret;
    
    return 0;
}
