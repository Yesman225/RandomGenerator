#include <iostream>
#include <vector>
#include <memory>
#include "UniformGenerator/EcuyerCombined.h"
#include "ContinuousGenerator/Normal.h"
#include "DiscreteGenerator/Poisson.h"
#include "ContinuousGenerator/Exponential.h"

using namespace std;
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
    size_t seed1=1383377383;
    size_t seed2=783876393;
    size_t modulus1=2147483563;
    size_t modulus2=2147483399;
    size_t multiplier1=40014;
    size_t multiplier2=40692;

    
    shared_ptr<RandomGenerator> gen = make_shared<LinearCongruential> (seed1, multiplier1, 1000, modulus1);
    EcuyerCombined EcuyerGenerator(seed1, multiplier1,modulus1,
                                    seed2, multiplier2,modulus2);
    Normal n1(EcuyerGenerator, NormalAlgo::boxMuller);
    Normal n2(EcuyerGenerator, NormalAlgo::centralLimit);
    Normal n3(EcuyerGenerator, NormalAlgo::rejection);
    Poisson p1(EcuyerGenerator, 0.1, PoissonAlgo::first);
    Poisson p2(EcuyerGenerator, 0.1, PoissonAlgo::second);
    Exponential e1(EcuyerGenerator, 0.5, ExpoAlgo::rejection);
    Exponential e2(EcuyerGenerator, 0.5, ExpoAlgo::inverse);

    gen ->moments(1000);
    n1.moments(100000);
    n2.moments(100000);
    n3.moments(100000);
    p1.moments(100000);
    p2.moments(100000);
    e1.moments(100000);
    e2.moments(100000);
    }
