#ifndef IndeHashFunction_hpp
#define IndeHashFunction_hpp

#include <random>
#include <vector>
#include <climits>
#include <cmath>

class IndeHashFunction
{
public:
    IndeHashFunction(int gamma);
    
    void draw();
    unsigned long evaluate(unsigned long x);
    void pre_evaluate(const std::vector<unsigned long>& set);
    unsigned long post_evaluate(int i) const;
    
private:
    int gamma;
    
    std::vector<unsigned long> polynomial;
    std::vector<unsigned long> pre_value;
    
    std::random_device rd;
    std::uniform_int_distribution<unsigned long> coef_dist;
    std::default_random_engine gen;
};

#endif /* IndeHashFunction_hpp */
