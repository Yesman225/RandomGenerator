#include "RandomGenerator.h"
#include <stdexcept>
#include <vector>
#include <iostream>


std::vector<double> RandomGenerator::moments(unsigned long nbSim, bool print)
{
    if (nbSim == 0) {
        throw std::invalid_argument("nbSim must be >0");
    }
    double sum = 0.0;
    double sum2 = 0.0;
    double x = 0;
    std::vector<double> v;

    for (unsigned long i = 0; i < nbSim; ++i){
        x = generate();
        sum += x;
        sum2 += x*x;
    }

    double mean = sum/nbSim;
    double var = (sum2/nbSim) - (mean*mean);
    v.assign({mean, var});
    if (print==true){ std::cout <<"mean: " << mean
              << ", var: " << var << "\n";
    }
    return v;
}


