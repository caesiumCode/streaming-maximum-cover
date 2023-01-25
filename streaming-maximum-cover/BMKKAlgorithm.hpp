#ifndef BMKKAlgorithm_hpp
#define BMKKAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class BMKKAlgorithm : public Algorithm
{
public:
    BMKKAlgorithm();
    virtual ~BMKKAlgorithm();
    
    virtual void setStream(Stream* stream)  {this->stream = stream;}
    virtual void setK(int k)                {this->k = k;}
    void setMaxSetSize(int max_set_size)    {this->max_set_size = max_set_size;}
    void setEpsilon(float epsilon)          {this->epsilon = epsilon;}
    
    virtual Result run();
    
private:
    Stream* stream;
    int k, max_set_size;
    float epsilon;
    
private:
    struct GuessState
    {
        float v;
        std::vector<int> I;
        std::unordered_set<ulong> C;
    };
};

}
#endif /* BMKKAlgorithm_hpp */
