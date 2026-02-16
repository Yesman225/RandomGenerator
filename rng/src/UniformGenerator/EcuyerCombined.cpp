#include "UniformGenerator/EcuyerCombined.h"
#include <stdexcept>
#include <cmath>

double EcuyerCombined::generate(){
    const double m1 = lcg1_.getModulus();
    const double z = lcg1_.generateRaw() - lcg2_.generateRaw();
    
    double x = std::fmod(z, m1-1.0);
    if (x < 0.0) x += m1;

    if (x > 0.0) return x / m1;
    return (m1-1) / m1; // x == 0
}
