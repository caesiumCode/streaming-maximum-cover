//
//  SetStream.hpp
//  mp-msc
//
//  Created by Stephen Jaud on 21/8/2022.
//

#ifndef SetStream_hpp
#define SetStream_hpp

#include <fcntl.h>  // O_RDONLY
#include <unistd.h> // read
#include <cstring>  // memchr

#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <random>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>

class SetStream {
public:
    struct Info {
        std::string name;
        long size;
        int m;
        int n;
        int max_set_size;
    };
public:
    SetStream(std::string, std::string, char = ' ');
    
    bool read_set(int &, std::vector<unsigned long> &);
    std::pair<int, unsigned long> get_m_n();
    Info get_info();
    
    void reset();
    void terminate();
    
public:
    static void create(int m, int n, int left, int mode, std::string path, std::string name);
    
private:
    std::string name;
    
private:
    char delimiters[1];
    int current_id;
    
    static const auto BUFFER_SIZE = 1*1024*1024;
    int     fildes;
    char    rem[BUFFER_SIZE + 1];
    char    buf[BUFFER_SIZE + 1];
    size_t  rem_length;
    size_t  bytes_read;
    char*   curr_p;
    char*   prev_p;
};

#endif /* SetStream_hpp */
