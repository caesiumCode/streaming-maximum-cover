#ifndef BMKKAlgorithm_hpp
#define BMKKAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class BMKKAlgorithm : public Algorithm
{
public:
    BMKKAlgorithm();
    ~BMKKAlgorithm();
    
    void setStream(Stream* stream)          {this->stream = stream;}
    void setK(int k)                        {this->k = k;}
    void setMaxSetSize(int max_set_size)    {this->max_set_size = max_set_size;}
    void setEpsilon(float epsilon)          {this->epsilon = epsilon;}
    void setM(int m)                        {this->m = m;}
    
    Result run();
    
private:
    Stream* stream;
    int k, max_set_size, m;
    float epsilon;
    
private:
    struct GuessState
    {
        float v;
        std::unordered_set<int> I;
        std::unordered_set<ulong> C;
    };
};

}
#endif /* BMKKAlgorithm_hpp */
