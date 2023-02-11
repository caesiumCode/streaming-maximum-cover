#ifndef YYAlgorithm_hpp
#define YYAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class YYAlgorithm : public Algorithm
{
public:
    YYAlgorithm();
    ~YYAlgorithm();
    
    void setStream(Stream* stream)  {this->stream = stream;}
    void setK(int k)                {this->k = k;}
    
    Result run();
    
private:
    Stream* stream;
    int k;
};

}

#endif /* YYAlgorithm_hpp */
