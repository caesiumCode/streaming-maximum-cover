#include "Stream.hpp"

RealStream::RealStream(const std::string& path, const std::string& dataset)
{
    fd = open((path + dataset).c_str(), O_RDONLY);
    
    s = read(fd, buffer, BUFFER_SIZE);
    buffer_iter = &buffer[0];
    
    current_id = 0;
}

bool RealStream::read_set(int &id, std::vector<unsigned long>& set)
{
    id = current_id++;
    
    set.clear();
    
    do
    {
        if ((buffer_iter - buffer) == s-1)
        {
            s = read(fd, buffer, BUFFER_SIZE);
            buffer_iter = &buffer[0];
            if (s == 0) return false;
        }
        
        unsigned long x = strtoul(buffer_iter, &buffer_iter, 10);
        
        if ((buffer_iter - buffer) == s)
        {
            s = read(fd, buffer, BUFFER_SIZE);
            buffer_iter = &buffer[0];
            
            if (buffer_iter[0] >= '0' && buffer_iter[0] <= '9')
            {
                unsigned long x_right = strtoul(buffer_iter, &buffer_iter, 10);
                int x_right_size = int(buffer_iter - buffer);
                for (int i = 0; i < x_right_size; i++) x *= 10;
                x += x_right;
            }
        }
        
        set.push_back(x);
                
    } while (buffer_iter[0] != '\n');
    
    return true;
}

void RealStream::reset()
{
    lseek(fd, 0, SEEK_SET);
    
    s = read(fd, buffer, BUFFER_SIZE);
    buffer_iter = &buffer[0];
    
    current_id = 0;
}

void RealStream::terminate()
{
    close(fd);
}
