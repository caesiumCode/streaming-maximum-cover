#ifndef YYAlgorithm_hpp
#define YYAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class YYAlgorithm : public Algorithm
{
public:
    YYAlgorithm();
    virtual ~YYAlgorithm();
    
    virtual void setStream(Stream* stream)  {this->stream = stream;}
    virtual void setK(int k)                {this->k = k;}
    
    virtual Result run();
    
private:
    Stream* stream;
    int k;
};

}

#endif /* YYAlgorithm_hpp */
