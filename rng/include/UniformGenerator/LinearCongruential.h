#ifndef LINEARCONGRUENTIAL_H

#define LINEARCONGRUENTIAL_H
#include "PseudoGenerator.h"

class LinearCongruential : public PseudoGenerator {
    public:
        explicit LinearCongruential(double seed, double multiplier, double increment, double modulus):
            PseudoGenerator(seed),
            multiplier_(multiplier), increment_(increment), modulus_(modulus){}
            double generate() override;
            double generateRaw();
            double getModulus() const {return modulus_;};
    private:
        double multiplier_;
        double increment_;
        double modulus_;

};
#endif
