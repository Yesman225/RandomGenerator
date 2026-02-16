#ifndef FINITESET_H
#define FINITESET_H

#include "DiscreteGenerator.h"
#include "UniformGenerator/UniformGenerator.h"
#include <vector>



class FiniteSet : public DiscreteGenerator {
    public:
        FiniteSet(UniformGenerator& u, const std::vector<double>& values, 
                    const std::vector<double>& probas); 

        double generate() override;
    

    private:
        UniformGenerator& u_;
        std::vector<double> values_;
        std::vector<double> probas_;
        std::vector<double> cumProbas_;
};
#endif
