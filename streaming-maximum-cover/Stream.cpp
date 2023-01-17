#include "Stream.hpp"

std::unordered_map<std::string, Info> Stream::load_infos(const std::string& path)
{
    std::unordered_map<std::string, Info> dataset_infos;
    
    std::ifstream file_infos(path + "dataset_infos.txt");
    
    std::string line;
    std::getline(file_infos, line);
    
    while (std::getline(file_infos, line))
    {
        std::istringstream streamline(line);
        std::string name;
        long size;
        int m, n, max;
        streamline >> name >> size >> m >> n >> max;
        dataset_infos[name] = {name, size, m, n, max};
    }
    
    file_infos.close();
    
    return dataset_infos;
}

Stream::Stream(const std::string& path, const std::string& dataset)
{
    fp = std::fopen((path + dataset).c_str(), "r");
    if(!fp) std::cout << "ERROR: could not open " << path << dataset << std::endl;
    
    current_id = 0;
}

bool Stream::read_set(int &id, std::vector<unsigned long>& set)
{
    id = current_id++;
    
    set.clear();
    
    char* buffer_iter = std::fgets(line_buffer, sizeof(line_buffer), fp);
    
    if (buffer_iter == nullptr) return false;
    
    while (*buffer_iter != '\n' && *buffer_iter != '\0')
    {
        if (*buffer_iter <= '9' &&
            *buffer_iter >= '0')
                set.push_back(std::strtoul(buffer_iter, &buffer_iter, 10));
        
        else    buffer_iter++;
    }
    
    /*
    
    do
    {
        if ((buffer_iter - buffer) == s-1)
        {
            s = std::read(fd, buffer, BUFFER_SIZE);
            buffer_iter = &buffer[0];
            if (s == 0) return false;
        }
        
        unsigned long x = std::strtoul(buffer_iter, &buffer_iter, 10);
        
        if ((buffer_iter - buffer) == s)
        {
            s = std::read(fd, buffer, BUFFER_SIZE);
            buffer_iter = &buffer[0];
            
            if (buffer_iter[0] >= '0' && buffer_iter[0] <= '9')
            {
                unsigned long x_right = std::strtoul(buffer_iter, &buffer_iter, 10);
                int x_right_size = int(buffer_iter - buffer);
                for (int i = 0; i < x_right_size; i++) x *= 10;
                x += x_right;
            }
        }
        
        set.push_back(x);
                
    } while (buffer_iter[0] != '\n');
     */
    
    return true;
}

void Stream::reset()
{
    std::rewind(fp);
    
    current_id = 0;
}

void Stream::terminate()
{
    std::fclose(fp);
}
