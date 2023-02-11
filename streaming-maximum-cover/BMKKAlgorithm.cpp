/*
 Badanidiyuru, Mirzasoleiman, Karbasi and Krause Algorithm
 https://dl.acm.org/doi/10.1145/2623330.2623637
 */

#include "BMKKAlgorithm.hpp"

using namespace smc;

BMKKAlgorithm::BMKKAlgorithm()
{

}

BMKKAlgorithm::~BMKKAlgorithm()
{
    
}

Result BMKKAlgorithm::run()
{
    Result result;
    
    auto START = high_resolution_clock::now();
    
    // Generate guesses
    
    std::vector<GuessState> O;
    float min_v = 1;
    while (min_v < max_set_size) min_v *= 1+epsilon;
    
    for (float v = min_v; v <= 2.f*float(k)*float(max_set_size); v *= 1+epsilon)
    {
        O.push_back(GuessState());
        O.back().v = v;
        O.back().C.clear();
        O.back().I.clear();
    }
    
    // One pass
    stream->reset();
    int id;
    std::vector<ulong> set;
    while (stream->read_set(id, set))
    {
        for (GuessState& guess : O) if (guess.I.size() < k)
        {
            float contrib = 0;
            float threshold = (guess.v/2.f - float(guess.C.size()))/float(k - guess.I.size());
            for (ulong x : set) if (!guess.C.contains(x)) contrib++;
            
            if (contrib >= threshold)
            {
                guess.I.insert(id);
                guess.C.insert(set.begin(), set.end());
            }
        }
    }
    
    // Find best guess
    int best_i = 0;
    for (int i = 1; i < O.size(); i++) if (O[i].C.size() > O[best_i].C.size()) best_i = i;
    
    const GuessState& state = O[best_i];
    
    auto END = high_resolution_clock::now();
    
    // Result
    
    result.space = 0;
    for (const GuessState& guess : O) result.space += guess.C.size();
    
    result.indices = std::vector<int>(state.I.begin(), state.I.end());
    result.coverage_size = state.C.size();
    result.time_tot = END - START;
    result.type = "BMKK";
    result.time_sub = std::chrono::nanoseconds(0);
    
    return result;
}
