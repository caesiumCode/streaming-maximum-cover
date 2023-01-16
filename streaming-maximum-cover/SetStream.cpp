#include "SetStream.hpp"

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

SetStream::SetStream(std::string path, std::string name_, char del)
{
    fildes = open((path+name_).c_str(), O_RDONLY);
    bytes_read = read(fildes, buf, BUFFER_SIZE);
    curr_p = buf;
    prev_p = buf;
    rem_length = 0;
    
    delimiters[0] = del;
    current_id = 0;
    name = name_;
}

bool SetStream::read_set(int & id, std::vector<unsigned long> & set)
{
    set.clear();
    
    if (!bytes_read) return false;
    
    prev_p = curr_p;
    curr_p = (char*) memchr(curr_p, '\n', (buf + bytes_read) - curr_p);
    if (!curr_p)
    {
        rem_length = (buf + BUFFER_SIZE) - prev_p;
        memcpy(rem, prev_p, rem_length);
        
        bytes_read = read(fildes, buf, BUFFER_SIZE);
        
        curr_p = buf;
        
        return read_set(id, set);
    }
    
    id = current_id;
    
    size_t  length = curr_p - prev_p;
    char    temp[rem_length + length + 1];
    memcpy(temp, rem, rem_length);
    memcpy(temp + rem_length, prev_p, length);
    temp[rem_length + length] = '\0';
    
    char* token = strtok(temp, delimiters);
    while(token != NULL)
    {
        set.push_back(atol(token));
        token = strtok(NULL, delimiters);
    }
    
    rem_length = 0;
    current_id++;
    curr_p++;
    
    return true;
}

std::pair<int, unsigned long> SetStream::get_m_n()
{
    std::vector<unsigned long> set;
    int m = 0;
    unsigned long n = 0;
    int id;
    
    reset();
    while (read_set(id, set))
    {
        m++;
        n = std::max(n, *std::max_element(set.begin(), set.end()));
    }
    reset();
    
    return {m, n};
}

SetStream::Info SetStream::get_info()
{
    /*
    Info info = {0, 0, 0};
    std::vector<int> set;
    int id;
    
    reset();
    while (read_set(id, set))
    {
        info.m++;
        info.n = std::max(info.n, *std::max_element(set.begin(), set.end()));
        info.max_set_size = std::max(info.max_set_size, (int)set.size());
    }
    reset();
     */
    
    std::vector<std::string> parsed_name = split(name, "-");
    
    if (parsed_name[0].substr(0, 4) == "mode")
    {
        SetStream::Info dataset_info;
        dataset_info.name = name;
        dataset_info.n = std::stoi(parsed_name[1]);
        dataset_info.m = std::stoi(parsed_name[2]);
        dataset_info.max_set_size = std::stoi(parsed_name[3]);
        dataset_info.size = 0;
        
        return dataset_info;
    }
    
    std::unordered_map<std::string, SetStream::Info> dataset_infos;
    dataset_infos["chess.dat"]          = {"chess.dat",         330970,         3196,       75,         37};
    dataset_infos["mushroom.dat"]       = {"mushroom.dat",      528190,         8124,       119,        23};
    dataset_infos["pumsb_star.dat"]     = {"pumsb_star.dat",    8343542,        49046,      2088,       76};
    dataset_infos["Accidents.dat"]      = {"Accidents.dat",     34946269,       340183,     468,        51};
    dataset_infos["Patents.dat"]        = {"Patents.dat",       124386653,      2089345,    3258983,    770};
    dataset_infos["Webdocs.dat"]        = {"Webdocs.dat",       1481705359,     1692082,    5267656,    71472};
    dataset_infos["Webbase.dat"]        = {"Webbase.dat",       5983780218,     56998289,   112195266,  3841};
    dataset_infos["SocialNetwork.dat"]  = {"SocialNetwork.dat", 15640890842,    37551359,   64961029,   3615};
    dataset_infos["UKUnion.dat"]        = {"UKUnion.dat",       29792226977,    74117320,   126454248l, 22429};
    
    return dataset_infos[name];
}

void SetStream::reset()
{
    lseek(fildes, 0, SEEK_SET);
    bytes_read = read(fildes, buf, BUFFER_SIZE);
    curr_p = buf;
    prev_p = buf;
    current_id = 0;
    rem_length = 0;
}

void SetStream::terminate()
{
    close(fildes);
}

void SetStream::create(int m, int n, int left, int mode, std::string path, std::string name)
{
    int maxs = 0;
    float avg = 0;
    
    std::vector<int> to_include_init(n+1);
    std::iota(to_include_init.begin(), to_include_init.end(), 0);
    std::unordered_set<int> to_include(to_include_init.begin(), to_include_init.end());
    
    std::ofstream file;
    file.open(path + name);
    
    // random elements - hight size variability
    if (mode == 0)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> element_dist(0, n);
        std::uniform_int_distribution<int> index_dist(0, n);
        std::uniform_int_distribution<int> size_dist(left - 0.7*left/2, left + 0.7*left);
        std::default_random_engine gen(rd());
        
        int q = n / m;
        int r = n % m;
        
        for (int i = 0; i < m; i++)
        {
            std::cout << "\r" << float(i)/float(m)*100.f << "%" << std::flush;
            for (int j = 0; j < q + (i < r); j++)
            {
                int index = index_dist(gen);
                index = index % to_include.size();
                auto iter = to_include.begin();
                std::advance(iter, index);
                int x = *iter;
                
                file << x << ' ';
                
                to_include.erase(iter);
            }
            
            int size_left = size_dist(gen);
            if (size_left > left + 0.6*left) size_left = 3*left;
            
            for (int j = 0; j < size_left; j++) file << element_dist(gen) << ' ';
            file << '\n';
            
            int set_size = (q + (i < r)) + size_left;
            maxs = set_size > maxs ? set_size : maxs;
            avg += set_size;
        }
        
        std::cout << "\rmaximum set size " << maxs << std::endl;
    }
    // random elements - constant size
    else if (mode == 1)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> element_dist(0, n);
        std::uniform_int_distribution<int> index_dist(0, n);
        std::default_random_engine gen(rd());
        
        int q = n / m;
        int r = n % m;
        for (int i = 0; i < m; i++)
        {
            std::cout << float(i)/float(m)*100.f << "%";
            
            for (int j = 0; j < q + (j < r); j++)
            {
                int index = index_dist(gen);
                index = index % to_include.size();
                auto iter = to_include.begin();
                std::advance(iter, index);
                int x = *iter;
                
                file << x << ' ';
                
                to_include.erase(iter);
            }
            
            for (int j = 0; j < left; j++) file << element_dist(gen) << ' ';
            file << '\n';
            
            int set_size = (q + (i < r)) + left;
            maxs = set_size > maxs ? left : maxs;
            avg += set_size;
        }
        
        std::cout << "\rmaximum set size " << maxs << std::endl;
    }
    // correlated elements - hight size variability
    else if (mode == 2)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> size_dist(left - 0.7*left/2, left + 0.7*left);
        std::default_random_engine gen(rd());
        
        int x = 0;
        
        for (int i = 0; i < m; i++)
        {
            std::cout << "\r" << float(i)/float(m)*100.f << "%" << std::flush;
            
            int size_left = size_dist(gen);
            if (size_left > left + 0.6*left) size_left = 3*left;
            
            for (int j = 0; j < size_left; j++)
            {
                file << x << ' ';
                x = x == n ? 0 : x+1;
            }
            file << '\n';
            
            maxs = size_left > maxs ? size_left : maxs;
            avg += size_left;
        }
        
        std::cout << "\rmaximum set size " << maxs << std::endl;
    }
    // correlated elements - constant size
    else if (mode == 3)
    {
        std::random_device rd;
        std::default_random_engine gen(rd());
        
        int x = 0;
        
        for (int i = 0; i < m; i++)
        {
            std::cout << "\r" << float(i)/float(m)*100.f << "%" << std::flush;
                        
            for (int j = 0; j < left; j++)
            {
                file << x << ' ';
                x = x == n ? 0 : x+1;
            }
            file << '\n';
            
            maxs = left > maxs ? left : maxs;
            avg += left;
        }
        
        std::cout << "\rmaximum set size " << maxs << std::endl;
    }
    avg /= (float) m;
    
    file.close();
    
    const std::string sig = "-" + std::to_string(n) + "-" + std::to_string(m) + "-" + std::to_string(maxs) + "-" + std::to_string(avg) + "-" + std::to_string(time(nullptr));
    
    std::rename((path + name).c_str(), (path + name + sig).c_str());
}
