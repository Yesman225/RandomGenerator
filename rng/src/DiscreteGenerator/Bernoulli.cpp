#include "DiscreteGenerator/Bernoulli.h"
#include <stdexcept>

double Bernoulli::generate() {
    if (success_< 0.0 || success_ > 1.0) {throw std::invalid_argument("success probability must be in [0,1]");}
    return (u_.generate() < success_) ? 1.0 : 0.0;
}
