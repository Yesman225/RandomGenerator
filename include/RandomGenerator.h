#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H
#include <vector>

class RandomGenerator {
    public :
        virtual ~RandomGenerator() = default;
        virtual double generate() = 0; // Pure virtual function
        std::vector<double> moments(unsigned long nbSim, bool print=true);

};

#endif
