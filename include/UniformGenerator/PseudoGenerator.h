#ifndef PSEUDOGENERATOR_H
#define PSEUDOGENERATOR_H

#include "UniformGenerator.h"

class PseudoGenerator : public UniformGenerator {
    public:
        PseudoGenerator();
        PseudoGenerator(size_t seed): seed_(seed){}
        void setSeed(size_t seed) { seed_ = seed;}
        double getSeed() const{return seed_;}
    
    protected:
        size_t seed_;
};
#endif
