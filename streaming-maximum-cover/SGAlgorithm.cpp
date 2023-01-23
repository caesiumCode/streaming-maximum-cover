/*
 Saha & Getoor Algorithm
 https://epubs.siam.org/doi/10.1137/1.9781611972795.60
 */

#include "SGAlgorithm.hpp"

using namespace smc;

SGAlgorithm::SGAlgorithm()
{
    
}

SGAlgorithm::~SGAlgorithm()
{
    
}

Result SGAlgorithm::run()
{
    auto START = high_resolution_clock::now();
    
    std::vector<int> I(k);
    std::vector<std::vector<ulong>> T(k);
    std::unordered_map<ulong, int> count;
    count.clear();
    
    int i = 0;
    std::vector<ulong> set;
    
    stream->reset();
    while (stream->read_set(i, T[i]) && i < k)
    {
        I[i] = i;
        for (ulong x : T[i])
        {
            if (!count.contains(x)) count[x] = 1;
            else                    count[x]++;
        }
    }
    
    std::vector<ulong> R(k);
    
    while (stream->read_set(i, set))
    {
        ulong B = 0;
        
        for (ulong x : set)
        {
            if (!count.contains(x))
            {
                B++;
                count[x] = 1;
            }
            else count[x]++;
        }
        
        int j_min = 0;
        for (int j = 0; j < k; j++)
        {
            R[j] = 0;
            
            for (ulong x : T[j]) if (count[x] == 1) R[j]++;
            
            if (R[j] < R[j_min]) j_min = j;
        }
        
        ulong Rmin = R[j_min];
        
        if (B >= 2 * Rmin)
        {
            for (ulong x : T[j_min])
            {
                count[x]--;
                if (count[x] == 0) count.erase(x);
            }
            
            I[j_min] = i;
            T[j_min] = set;
        }
        else
        {
            for (ulong x : set)
            {
                count[x]--;
                if (count[x] == 0) count.erase(x);
            }
        }
    }
    
    auto END = high_resolution_clock::now();
    
    Result result;
    result.type = "SG";
    result.indices = I;
    result.coverage_size = (ulong)count.size();
    result.space = 0;
    for (int i = 0; i < k; i++) result.space += T[i].size();
    result.time_tot = END - START;
    result.time_sub = std::chrono::nanoseconds(0);
    
    return result;
}
