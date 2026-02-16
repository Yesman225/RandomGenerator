#include <iostream>
#include <vector>
#include "UniformGenerator/EcuyerCombined.h"
#include "ContinuousGenerator/Normal.h"
#include "DiscreteGenerator/Poisson.h"
#include "ContinuousGenerator/Exponential.h"


#ifdef TEST
class ConstantGenerator : public RandomGenerator {
    public:
        explicit ConstantGenerator(double v) : value(v) {}
        double generate() override { return value ;}
    private:
        double value;
};
#endif

int main() {
    double seed1=1383377383;
    double seed2=783876393;
    double modulus1=2147483563;
    double modulus2=2147483399;
    double multiplier1=40014;
    double multiplier2=40692;

    
    EcuyerCombined EcuyerGenerator(seed1, multiplier1,modulus1,
                                    seed2, multiplier2,modulus2);
    Normal n1(EcuyerGenerator, NormalAlgo::boxMuller);
    Normal n2(EcuyerGenerator, NormalAlgo::centralLimit);
    Normal n3(EcuyerGenerator, NormalAlgo::rejection);
    Poisson p1(EcuyerGenerator, 0.1, PoissonAlgo::first);
    Poisson p2(EcuyerGenerator, 0.1, PoissonAlgo::second);
    Exponential e1(EcuyerGenerator, 0.5, ExpoAlgo::rejection);
    Exponential e2(EcuyerGenerator, 0.5, ExpoAlgo::inverse);


    n1.moments(100000);
    n2.moments(100000);
    n3.moments(100000);
    p1.moments(100000);
    p2.moments(100000);
    e1.moments(100000);
    e2.moments(100000);

    }
