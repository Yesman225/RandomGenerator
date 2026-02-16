#include "ContinuousGenerator/Exponential.h"
#include <stdexcept>
#include <cmath>
#include <limits>

Exponential::Exponential(UniformGenerator& u, double lambda, ExpoAlgo algo, double b):
            b_(b), u_(u),lambda_(lambda),algo_(algo){
                if(lambda_<=0) throw std::invalid_argument("Lambda should be higher than 0");
                }

double Exponential::generate() {
    if (algo_ == ExpoAlgo::inverse) {
        return generateInverse();
    }
    
    if (algo_ == ExpoAlgo::rejection){
        return generateRejection();
    }

    throw std::invalid_argument("Algorithm not valid");
}

double Exponential::generateInverse(){
    double u = u_.generate();
    if (u <= 0.0) u = std::numeric_limits<double>::min();
    return -std::log(u) / lambda_;
}

double Exponential::generateRejection(){
    for(;;){
        const double x = b_ * u_.generate();
        const double y= lambda_ * u_.generate();
        
        if(y<= lambda_ * std::exp(-lambda_ * x)){
            return x;
        }
    }
}
