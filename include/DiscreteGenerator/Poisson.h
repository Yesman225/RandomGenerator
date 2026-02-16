#ifndef POISSON_H
#define POISSON_H

#include "DiscreteGenerator.h"
#include "UniformGenerator/UniformGenerator.h"
#include "ContinuousGenerator/Exponential.h"

enum class PoissonAlgo {
    first,
    second
};

class Poisson : public DiscreteGenerator {
    public:
        Poisson(UniformGenerator&u, double lambda, PoissonAlgo algoPoisson, ExpoAlgo algoExponential = ExpoAlgo::inverse);
        double generate() override;
    
    private:
        Exponential e_;
        double generateInverse();
        double generateExpo();
        UniformGenerator& u_;
        double lambda_;
        PoissonAlgo algoPoisson_;
};
#endif
