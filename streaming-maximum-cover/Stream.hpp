#ifndef Stream_hpp
#define Stream_hpp

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <fcntl.h> 
#include <unistd.h>
#include <cstring>

struct Info {
    std::string name;
    long size;
    int m;
    int n;
    int max_set_size;
};

class Stream
{
public:
    Stream(const std::string& path, const std::string& dataset);
    
    bool read_set(int& id, std::vector<unsigned long>& set);
    void reset();
    void terminate();
    
    static std::unordered_map<std::string, Info> load_infos(const std::string& path);
    
private:
    int current_id;
    
private:
    static const int BUFFER_SIZE = 1024 * 1024;
    
    int fd;
    char buffer[BUFFER_SIZE];
    char* buffer_iter;
    ssize_t s;
};

#endif /* Stream_hpp */
