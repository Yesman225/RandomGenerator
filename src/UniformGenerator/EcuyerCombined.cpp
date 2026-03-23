#include "UniformGenerator/EcuyerCombined.h"
#include <stdexcept>
#include <cmath>

EcuyerCombined::EcuyerCombined(double seed1, double multiplier1, double modulus1,
                double seed2, double multiplier2, double modulus2):
    PseudoGenerator(0.0),
    lcg1_(seed1, multiplier1, 0.0, modulus1),
    lcg2_(seed2, multiplier2, 0.0, modulus2){}

double EcuyerCombined::generate(){
    const double m1 = lcg1_.getModulus();
    const double z = lcg1_.generateRaw() - lcg2_.generateRaw();
    
    double x = std::fmod(z, m1-1.0);
    if (x < 0.0) x += m1;

    if (x > 0.0) return x / m1;
    return (m1-1) / m1; // x == 0
}
