#ifndef PSEUDOGENERATOR_H
#define PSEUDOGENERATOR_H

#include "UniformGenerator.h"

class PseudoGenerator : public UniformGenerator {
    public:
        explicit PseudoGenerator(double seed): seed_(seed){}
        void setSeed(double seed) { seed_ = seed;}
        double getSeed() const{return seed_;}
    
    protected:
        double seed_;
};
#endif
