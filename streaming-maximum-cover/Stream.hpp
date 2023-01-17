#ifndef Stream_hpp
#define Stream_hpp

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <cstdio>
#include <cstdlib>

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
    static const int LINE_BUFFER_SIZE = 2 << 18;
    
    FILE* fp;
    char line_buffer[LINE_BUFFER_SIZE];
};

#endif /* Stream_hpp */
