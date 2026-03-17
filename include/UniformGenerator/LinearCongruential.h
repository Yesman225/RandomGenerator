#ifndef LINEARCONGRUENTIAL_H

#define LINEARCONGRUENTIAL_H
#include "PseudoGenerator.h"

class LinearCongruential : public PseudoGenerator {
    public:
            LinearCongruential();
            LinearCongruential(size_t seed, size_t multiplier, size_t  increment, size_t modulus):
            PseudoGenerator(seed),
            multiplier_(multiplier), increment_(increment), modulus_(modulus){}
            double generate() override;
            double generateRaw();
            double getModulus() const {return modulus_;};
    private:
        size_t multiplier_;
        size_t increment_;
        size_t modulus_;

};
#endif
