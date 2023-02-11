#ifndef MGVOAlgorithm_hpp
#define MGVOAlgorithm_hpp

#include "SMCAlgorithm.hpp"

#include "IndeHashFunction.hpp"

namespace smc
{

class MGVOAlgorithm : public Algorithm
{
public:
    MGVOAlgorithm();
    ~MGVOAlgorithm();
    
    void setStream(Stream* stream)          {this->stream = stream;}
    void setInde(IndeType inde)             {this->inde = inde;}
    void setC(float c)                      {this->c = c;}
    void setEpsilon(float epsilon)          {this->epsilon = epsilon;}
    void setK(int k)                        {this->k = k;}
    void setM(int m)                        {this->m = m;}
    void setN(ulong n)                      {this->n = n;}
    void setMaxSetSize(int max_set_size)    {this->max_set_size = max_set_size;}
    
    Result run();
    
private:
    Stream* stream;
    IndeType inde;
    float c;
    float epsilon;
    int m;
    ulong n;
    int k;
    int max_set_size;
    
private:
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
        std::unordered_set<ulong> C;
        std::vector<std::pair<int, std::vector<ulong>>> W;
    };
};

}

#endif /* MGVOAlgorithm_hpp */
