#ifndef ECUYERCOMBINED_H
#define ECUYERCOMBINED_H

#include "LinearCongruential.h"

class EcuyerCombined : public PseudoGenerator {
    public:
    EcuyerCombined(double seed1, double multiplier1, double modulus1,
                    double seed2, double multiplier2, double modulus2):
        PseudoGenerator(0.0),
        lcg1_(seed1, multiplier1, 0.0, modulus1),
        lcg2_(seed2, multiplier2, 0.0, modulus2){}
    
    double generate() override;
    private:
        LinearCongruential lcg1_;
        LinearCongruential lcg2_;
};
#endif
