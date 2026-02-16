#include "DiscreteGenerator/Binomial.h"
#include <stdexcept>

double Binomial::generate() {
    unsigned int sum = 0;
    for (unsigned int i =0; i<n_; ++i) {
        sum += static_cast<unsigned int> (b_.generate());
    }
    return static_cast<double>(sum);
}
