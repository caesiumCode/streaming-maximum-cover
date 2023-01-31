#ifndef SMCAlgorithm_hpp
#define SMCAlgorithm_hpp

#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>

#include "Stream.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

using ulong = unsigned long;

namespace smc
{

struct Result
{
    Result()
    {
        indices.clear();
        coverage_size = 0;
        
        type = "";
        
        space = 0;
        time_sub = std::chrono::nanoseconds(0);
        time_tot = std::chrono::nanoseconds(0);
    }
    
    std::string type;
    
    // Solution
    std::vector<int> indices;
    ulong coverage_size;
    
    // Tracking
    long space;
    std::chrono::nanoseconds time_sub;
    std::chrono::nanoseconds time_tot;
    
    std::string to_string()
    {
        std::string output;
        
        output += type + ",";
        output += std::to_string(indices.size()) + ",";
        output += std::to_string(coverage_size) + ",";
        output += std::to_string(space) + ",";
        output += std::to_string(duration<double, std::milli>(time_sub).count()) + ",";
        output += std::to_string(duration<double, std::milli>(time_tot).count());
        
        return output;
    }
};

class Algorithm
{
public:
    Algorithm();
    virtual ~Algorithm();
    
    virtual void setStream(Stream* stream) = 0;
    virtual void setK(int k) = 0;
    
    virtual Result run() = 0;
};

}

#endif /* SMCAlgorithm_hpp */
