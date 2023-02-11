/*
 Ashkan Norouzi-Fard, Jakub Tarnawski, Slobodan Mitrovic, Amir Zandieh, Aidasadat Mousavifar, Ola Svensson Algorithm
 https://arxiv.org/pdf/1808.01842.pdf
 */

#include "AJSAAOAlgorithm.hpp"

using namespace smc;

AJSAAOAlgorithm::AJSAAOAlgorithm()
{

}

AJSAAOAlgorithm::~AJSAAOAlgorithm()
{
    
}

Result AJSAAOAlgorithm::run()
{
    Result result;
    
    auto START = high_resolution_clock::now();
        
    
    // Set guesses
    std::vector<GuessState> O;
    float min_v = max_set_size / (1.f+epsilon);
    
    for (float v = min_v; v <= float(k)*float(max_set_size); v *= 1.f+epsilon)
    {
        O.push_back(GuessState());
        O.back().v = v;
        O.back().r = 2.f*v/(3.f*float(k));
        O.back().C.clear();
        O.back().I.clear();
    }
    
    
    int id;
    std::vector<ulong> set;
    
    // First pass
    stream->reset();
    while (stream->read_set(id, set))
    {
        for (GuessState& guess : O) if (guess.I.size() < k)
        {
            float contrib = 0;
            for (ulong x : set) if (!guess.C.contains(x)) contrib++;
            
            if (contrib >= guess.r)
            {
                guess.I.insert(id);
                guess.C.insert(set.begin(), set.end());
            }
        }
    }
    
    
    // Second pass
    for (GuessState& guess : O) guess.r = 4.f*guess.v/(9.f*float(k));
    
    stream->reset();
    while (stream->read_set(id, set))
    {
        for (GuessState& guess : O) if (guess.I.size() < k)
        {
            float contrib = 0;
            for (ulong x : set) if (!guess.C.contains(x)) contrib++;
            
            if (contrib >= guess.r)
            {
                guess.I.insert(id);
                guess.C.insert(set.begin(), set.end());
            }
        }
    }
    
    // Find best guess
    
    int best_i = 0;
    for (int i = 1; i < O.size(); i++) if (O[i].C.size() > O[best_i].C.size()) best_i = i;
    
    GuessState& state = O[best_i];
    
    auto END = high_resolution_clock::now();
    
    // Result
    
    result.space = 0;
    for (const GuessState& guess : O) result.space += guess.C.size();
    
    result.indices = std::vector<int>(state.I.begin(), state.I.end());
    result.coverage_size = state.C.size();
    result.time_tot = END - START;
    result.type = "AJSAAO";
    result.time_sub = std::chrono::nanoseconds(0);
    
    return result;
}
