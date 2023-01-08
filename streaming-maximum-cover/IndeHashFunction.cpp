#include "IndeHashFunction.hpp"

IndeHashFunction::IndeHashFunction(int gamma)
{
    this->gamma = gamma;
    
    coef_dist = std::uniform_int_distribution<unsigned long>(0, ULONG_MAX-1);
    gen = std::default_random_engine(rd());
    
    polynomial.resize(gamma);
    draw();
}

void IndeHashFunction::draw()
{
    if (gamma > 0)
    {
        polynomial[0] = coef_dist(gen);
        while (polynomial[0] == 0) polynomial[0] = coef_dist(gen);
        
        for (int i = 1; i < gamma; i++) polynomial[i] = coef_dist(gen);
    }
    else polynomial = {0};
}

unsigned long IndeHashFunction::evaluate(unsigned long x)
{
    unsigned long y = 0;
    unsigned long lx = x;
    
    for (int i = 0; i < gamma; i++) y = lx*y + polynomial[i];
    
    return y;
}

void IndeHashFunction::pre_evaluate(const std::vector<unsigned long>& set)
{
    int n = (int)set.size();
    
    pre_value.resize(n);
    std::fill(pre_value.begin(), pre_value.end(), 0);
        
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < gamma; i++) pre_value[j] = set[j]*pre_value[j] + polynomial[i];
    }
}

unsigned long IndeHashFunction::post_evaluate(int i) const
{
    return pre_value[i];
}
