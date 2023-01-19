#ifndef SMCAlgorithm_hpp
#define SMCAlgorithm_hpp

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <chrono>
#include <iostream>

#include "Stream.hpp"
#include "IndeHashFunction.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

namespace smc
{

enum IndeType {
    FULL,
    OPT,
    PAIRWISE,
    FULLSAMP
};

struct Result
{
    Result()
    {
        indices.clear();
        coverage_size = 0;
        
        lambda = 0;
        gamma = 0;
        inde = "";
        
        n_passes = 0;
        n_guesses = 0;
        space = 0;
        time_sub = std::chrono::nanoseconds(0);
        time_tot = std::chrono::nanoseconds(0);
    }
    
    // Solution
    std::vector<int> indices;
    int coverage_size;
    
    // Parameters
    float lambda;
    float gamma;
    std::string inde;
    
    // Tracking
    int n_passes;
    int n_guesses;
    long space;
    std::chrono::nanoseconds time_sub;
    std::chrono::nanoseconds time_tot;
    
    std::string to_string()
    {
        std::string output;
        
        output += std::to_string(indices.size()) + ",";
        output += std::to_string(coverage_size) + ",";
        output += std::to_string(lambda) + ",";
        output += std::to_string(gamma) + ",";
        output += inde + ",";
        output += std::to_string(n_passes) + ",";
        output += std::to_string(n_guesses) + ",";
        output += std::to_string(space) + ",";
        output += std::to_string(duration<double, std::milli>(time_sub).count()) + ",";
        output += std::to_string(duration<double, std::milli>(time_tot).count());
        
        return output;
    }
};

struct GuessState
{
    bool active;
    bool wrong;
    long n_elements;
    
    unsigned long v;
    float lambda;
    float z;
    float threshold;
    
    std::unordered_set<int> I;
    std::unordered_set<unsigned long> C;
};

class Algorithm
{
public:
    Algorithm();
    void setStream(Stream* stream)          {this->stream = stream;}
    void setInde(IndeType inde)             {this->inde = inde;}
    void setC(float c)                      {this->c = c;}
    void setEpsilon(float epsilon)          {this->epsilon = epsilon;}
    void setK(int k)                        {this->k = k;}
    void setM(int m)                        {this->m = m;}
    void setN(int n)                        {this->n = n;}
    void setMaxSetSize(int max_set_size)    {this->max_set_size = max_set_size;}
    
    Result run();
    
private:
    Stream* stream;
    IndeType inde;
    float c;
    float epsilon;
    int k;
    int m;
    int n;
    int max_set_size;
};

}

#endif /* SMCAlgorithm_hpp */
