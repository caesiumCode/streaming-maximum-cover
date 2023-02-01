/*
 McGregor and Vu Algorithm
 https://link.springer.com/article/10.1007/s00224-018-9878-x
 */

#include "MGVAlgorithm.hpp"

using namespace smc;

MGVAlgorithm::MGVAlgorithm()
{
    
}

MGVAlgorithm::~MGVAlgorithm()
{
    
}

Result MGVAlgorithm::run()
{
    auto Tstart = high_resolution_clock::now();
    
    Result result;
    
    int gamma;
    switch (inde) {
        case IndeType::FULL:
            gamma = std::ceil(2.f * c * float(k) * std::log(m) / (epsilon * epsilon));
            gamma = std::max(2, gamma);
            result.type = "MGVU-full";
            break;
            
        case IndeType::OPT:
            gamma = std::ceil(c * float(k) * std::log(m) / 3.f);
            gamma = std::max(2, gamma);
            result.type = "MGVU-opt";
            break;
            
        case IndeType::PAIRWISE:
            gamma = 2;
            result.type = "MGVU-pairwise";
            break;
            
        case IndeType::FULLSAMP:
            gamma = 0;
            result.type = "MGVU-fullsamp";
            break;
            
        default:
            gamma = 0;
            result.type = "MGVU-fullsamp";
            break;
    }
    IndeHashFunction hash(gamma);
    int activity = 0;
        
    /*
     Set guesses
     */
    std::vector<GuessState> guess_states;
    unsigned long v = max_set_size/2;
    while (v <= max_set_size*k && v <= n)
    {
        guess_states.push_back(GuessState());
        guess_states.back().v = v;
        activity++;
        
        v *= 2;
    }
        
    for (GuessState& state : guess_states)
    {
        state.active = true;
        state.wrong = false;
        state.n_elements = 0;
        state.lambda = c * float(k) * std::log(m) / (epsilon * epsilon);
        if (inde == IndeType::FULLSAMP || state.lambda > state.v) state.lambda = state.v;

        state.z = 2.f * (1.f + epsilon) * state.lambda;
        state.threshold = state.z / float(k);
        
        state.I.clear();
        state.C.clear();
    }
        
    /*
     Thresholding
     */
    int j_max = 1 + std::ceil(std::log(4.f * M_E) / std::log(1.f + epsilon));
    for (int j = 1; activity > 0 && j <= j_max; j++)
    {        
        stream->reset();
        int id;
        std::vector<unsigned long> set;
        while (activity > 0 && stream->read_set(id, set))
        {
            if (inde != IndeType::FULLSAMP)
            {
                auto T1 = high_resolution_clock::now();
                hash.pre_evaluate(set);
                auto T2 = high_resolution_clock::now();
                result.time_sub += T2 - T1;
            }

            // Iterate over guesses
            for (GuessState& state : guess_states) if (state.active && !state.I.contains(id))
            {
                // Calculate S\C
                std::vector<unsigned long> sub_set;
                if (state.lambda < state.v)
                {
                    for (int i = 0; i < set.size(); i++) if (!state.C.contains(set[i]))
                    {
                        auto T1 = high_resolution_clock::now();
                        unsigned long hx = hash.post_evaluate(i);
                        if ((hx % state.v) < state.lambda) sub_set.push_back(set[i]);
                        auto T2 = high_resolution_clock::now();
                        
                        result.time_sub += T2 - T1;
                    }
                }
                else for (unsigned long x : set) if (!state.C.contains(x)) sub_set.push_back(x);
                
                // Check contribution
                if (sub_set.size() >= state.threshold && state.I.size() < k)
                {
                    state.I.insert(id);
                    for (unsigned long x : sub_set) state.C.insert(x);
                }
                
                // Termination conditions
                if (state.C.size() > state.z)
                {
                    state.wrong = true;
                    state.active = false;
                    activity--;
                }
                else if (state.I.size() >= k)
                {
                    state.active = false;
                    activity--;
                }
            }
        }
        
        for (GuessState& state : guess_states) state.threshold /= 1.f + epsilon;
    }
        
    // Fisrt estimate
    int max_i = 0;
    double max_p = guess_states[max_i].lambda / double(guess_states[max_i].v);
    for (int i = 0; i < guess_states.size(); i++)
    {
        double p = guess_states[max_i].lambda / double(guess_states[max_i].v);
        if (double(guess_states[i].C.size()) / p > double(guess_states[max_i].C.size()) / max_p)
        {
            max_i = i;
            max_p = p;
        }
    }
    int fst_max_i = max_i;
    
    // Biggest |I|
    max_i = 0;
    while (max_i < guess_states.size() && guess_states[max_i].wrong) max_i++;
    for (int i = 0; i < guess_states.size(); i++)
    {
        if (!guess_states[i].wrong && guess_states[i].I.size() >= guess_states[max_i].I.size()) max_i = i;
    }
        
    // Find right guess
    /*
    for (GuessState& state : guess_states) if (state.C.size() < (1.f - epsilon)*(1.f-1.f/M_E-epsilon)*state.lambda)
        state.wrong = true;
    
    max_i = (int)guess_states.size()-1;
    while (max_i >= 0 && guess_states[max_i].wrong) max_i--;
    */
     
    GuessState state = guess_states[max_i];
     /*
    if (max_i < 0) state = guess_states[fst_max_i];
    else state = guess_states[max_i];
     */
        
    auto Tend = high_resolution_clock::now();

    std::set<unsigned long> true_C;
    
    if (gamma >= 2 || state.lambda < state.v)
    {
        stream->reset();
        
        std::vector<unsigned long> set;
        int id;
        while (stream->read_set(id, set)) if (state.I.contains(id)) true_C.insert(set.begin(), set.end());
    }
    else true_C.insert(state.C.begin(), state.C.end());
    
    result.indices.clear();
    for (int x : state.I) result.indices.push_back(x);
    result.coverage_size = (int)true_C.size();
    result.time_tot = Tend - Tstart;
    result.space = 0;
    for (const GuessState& state : guess_states) result.space += state.C.size();
    
    return result;
}
