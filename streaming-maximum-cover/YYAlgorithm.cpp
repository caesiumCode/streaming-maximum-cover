/*
 Yu & Yuan Algorithm
 https://epubs.siam.org/doi/10.1137/1.9781611972832.84
 */

#include "YYAlgorithm.hpp"

using namespace smc;

YYAlgorithm::YYAlgorithm()
{

}

YYAlgorithm::~YYAlgorithm()
{
    
}

struct SetWrapper
{
    int id;
    std::unordered_set<ulong> set;
};
using pSetWrapper = std::shared_ptr<SetWrapper>;

struct cmp
{
    bool operator() (pSetWrapper S1, pSetWrapper S2) const
    {
        return (S1->set.size() > S2->set.size()) || (S1->set.size() == S2->set.size() && S1->id < S2->id);
    }
};

void SIFT(int id,
          std::vector<ulong>& set,
          std::unordered_set<pSetWrapper>& C,
          std::unordered_map<ulong, pSetWrapper>& element_set,
          float beta)
{
    pSetWrapper curr = std::make_shared<SetWrapper>();
    curr->id = id;
    curr->set.insert(set.begin(), set.end());
            
    // insert S
    bool inserted = false;
    float threshold = 0;
    while (!inserted && !curr->set.empty())
    {
        threshold = float(curr->set.size())/beta;
        
        std::vector<ulong> to_erase;
        for (ulong x : curr->set)
        {
            std::unordered_map<ulong, pSetWrapper>::iterator iT = element_set.find(x);
            if (iT != element_set.end())
            {
                const std::unordered_set<ulong>& T = iT->second->set;
                
                if (float(T.size()) >= float(curr->set.size())/beta) for (ulong y : T) to_erase.push_back(y);
            }
        }
        
        for (ulong x : to_erase) curr->set.erase(x);
        
        if (float(curr->set.size()) >= threshold)
        {
            C.insert(curr);
            inserted = true;
        }
    }
    
    // update Ls
    if (!curr->set.empty())
    {
        for (ulong x : curr->set)
        {
            std::unordered_map<ulong, pSetWrapper>::iterator iT = element_set.find(x);
            if (iT != element_set.end())
            {
                std::unordered_set<ulong>& T = iT->second->set;

                T.erase(x);
                
                if (T.empty()) C.erase(iT->second);
            }
            
            element_set[x] = curr;
        }
    }
}

Result YYAlgorithm::run()
{
    Result result;
    
    auto START = high_resolution_clock::now();
    
    float beta = 1.1f;
    std::unordered_set<pSetWrapper> C;
    std::unordered_map<ulong, pSetWrapper> element_set;
    
    std::vector<ulong> set;
    int id;

    stream->reset();
    while (stream->read_set(id, set))
        SIFT(id, set, C, element_set, beta);
        
    std::set<pSetWrapper, cmp> sortC;
    sortC.insert(C.begin(), C.end());
    
    std::set<pSetWrapper, cmp>::iterator it;
    int i = 0;
    for (it = sortC.begin(); it != sortC.end(); it++)
    {
        if (i < k)
        {
            result.indices.push_back((*it)->id);
            result.coverage_size += (*it)->set.size();
        }
        
        result.space += (*it)->set.size();
        
        i++;
    }
    
    auto END = high_resolution_clock::now();
    
    result.time_tot = END - START;
    result.type = "YY";
    result.time_sub = std::chrono::nanoseconds(0);
    
    return result;
}
