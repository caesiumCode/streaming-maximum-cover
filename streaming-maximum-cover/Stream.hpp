#ifndef Stream_hpp
#define Stream_hpp

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h> 
#include <unistd.h>
#include <cstring>

class Stream
{
public:
    virtual bool read_set(int& id, std::vector<unsigned long>& set) = 0;
    virtual void reset() = 0;
    virtual void terminate() = 0;
};

class RealStream : public Stream
{
public:
    RealStream(const std::string& path, const std::string& dataset);
    
    bool read_set(int& id, std::vector<unsigned long>& set);
    void reset();
    void terminate();
    
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
