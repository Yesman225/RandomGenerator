#ifndef BERNOULLI_H
#define BERNOULLI_H

#include "DiscreteGenerator.h"
#include "UniformGenerator/UniformGenerator.h"


class Bernoulli : public DiscreteGenerator {
    public:
        Bernoulli(UniformGenerator& u, double success): 
        u_(u), success_(success){}
        double generate() override;
    
    private:
        UniformGenerator& u_;
        double success_;
};
#endif
