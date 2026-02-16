#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H

#include "ContinuousGenerator.h"
#include "UniformGenerator/UniformGenerator.h"

enum class ExpoAlgo{inverse, rejection};

class Exponential : public ContinuousGenerator{
    public:
        Exponential(UniformGenerator& u, double lambda, ExpoAlgo algo, double b = 10);
        
        double generate() override;

    private:
        double generateInverse();
        double generateRejection();
        double b_;
        UniformGenerator& u_;
        double lambda_;
        ExpoAlgo algo_;
};
#endif
