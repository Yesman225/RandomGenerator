#ifndef LINEARCONGRUENTIAL_H

#define LINEARCONGRUENTIAL_H
#include "PseudoGenerator.h"

class LinearCongruential : public PseudoGenerator {
    public:
            LinearCongruential();
            LinearCongruential(mylong seed, mylong multiplier, mylong  increment, mylong modulus);
            double generate() override;
            double generateRaw();
            double getModulus() const {return modulus_;};
    private:
        size_t multiplier_;
        size_t increment_;
        size_t modulus_;

};
#endif
