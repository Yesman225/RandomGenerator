#ifndef BINOMIAL_H
#define BINOMIAL_H

#include "DiscreteGenerator.h"
#include "DiscreteGenerator/Bernoulli.h"



class Binomial : public DiscreteGenerator {
    public:
        Binomial(UniformGenerator& u, double success, unsigned int n):
        b_(u,success), n_(n){}
        double generate() override;
    
    private:
        Bernoulli b_;
        unsigned int n_;
};
#endif
