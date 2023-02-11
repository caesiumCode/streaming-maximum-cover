#ifndef AJSAAOAlgorithm_hpp
#define AJSAAOAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class AJSAAOAlgorithm : public Algorithm
{
public:
    AJSAAOAlgorithm();
    ~AJSAAOAlgorithm();
    
    void setStream(Stream* stream)          {this->stream = stream;}
    void setK(int k)                        {this->k = k;}
    void setMaxSetSize(int max_set_size)    {this->max_set_size = max_set_size;}
    void setEpsilon(float epsilon)          {this->epsilon = 9.f*epsilon/5.f;}
    void setM(int m)                        {this->m = m;}
    
    virtual Result run();
    
private:
    Stream* stream;
    int k, max_set_size, m;
    float epsilon;
    
private:
    struct GuessState
    {
        float v;
        float r;
        std::unordered_set<int> I;
        std::unordered_set<ulong> C;
    };
};

}

#endif /* AJSAAOAlgorithm_hpp */
