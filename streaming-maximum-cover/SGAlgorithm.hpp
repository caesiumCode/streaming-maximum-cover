#ifndef SGAlgorithm_hpp
#define SGAlgorithm_hpp

#include "SMCAlgorithm.hpp"

namespace smc
{

class SGAlgorithm : public Algorithm
{
public:
    SGAlgorithm();
    ~SGAlgorithm();
    
    void setStream(Stream* stream)  {this->stream = stream;}
    void setK(int k)                {this->k = k;}
    
    Result run();
    
private:
    Stream* stream;
    int k;
};

}
#endif /* SGAlgorithm_hpp */
