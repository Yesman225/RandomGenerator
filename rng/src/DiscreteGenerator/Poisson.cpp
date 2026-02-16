#include "DiscreteGenerator/Poisson.h"
#include <cmath>
#include <stdexcept>

Poisson::Poisson(UniformGenerator&u, double lambda, PoissonAlgo algoPoisson, ExpoAlgo algoExponential):
    e_(u,lambda,algoExponential),u_(u), lambda_(lambda), algoPoisson_(algoPoisson){
        if(lambda_<0 && algoPoisson_== PoissonAlgo::first) throw std::invalid_argument("Lambda should be higher or equal to 0");
        if(lambda_<=0 && algoPoisson_== PoissonAlgo::second) throw std::invalid_argument("Lambda should be stricly higher than 0");
    }

double Poisson::generate(){
    if (algoPoisson_== PoissonAlgo::first){
        return Poisson::generateInverse();
    } else {
        return Poisson::generateExpo();
    }
}

double Poisson::generateInverse(){
    double U = u_.generate();
    double p = std::exp(-lambda_);
    double cdf = p;
    unsigned int k = 0;

    while (U>cdf){
        ++k;
        p = (lambda_/k)*p;
        cdf += p;
        };

    return static_cast<double>(k);
}

double Poisson::generateExpo(){
    double t = 0.0;
    unsigned int k = 0;
    while(true){
        double x = e_.generate();
        if (t + x > 1.0) break;
        t += x;
        ++k;}
    return k;
    
}
