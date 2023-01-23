#ifndef SGAlgorithm_hpp
#define SGAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class SGAlgorithm : public Algorithm
{
public:
    SGAlgorithm();
    virtual ~SGAlgorithm();
    
    virtual void setStream(Stream* stream)  {this->stream = stream;}
    virtual void setK(int k)                {this->k = k;}
    
    virtual Result run();
    
private:
    Stream* stream;
    int k;
};

}
#endif /* SGAlgorithm_hpp */
