#include "DiscreteGenerator/FiniteSet.h"
#include <cmath>
#include <stdexcept>
#include <cstddef>


FiniteSet::FiniteSet(UniformGenerator& u, const std::vector<double>& values, 
                    const std::vector<double>& probas): 
        u_(u), values_(values), probas_(probas) {
        
            if (values.empty() || values.size() != probas.size()) {
            throw std::invalid_argument("values/probas size mismatch or empty");
        }
       
            if(probas_[0]!=0) throw std::invalid_argument("first proba should be 0");
        
        double sum = 0.0;
        cumProbas_.reserve(probas_.size());
        
        for (double p : probas_) {
            if (p<0.0 || p>1.0) throw std::invalid_argument("probability should be in [0,1]");
            sum += p;
            cumProbas_.push_back(sum);
        }
        
        if(std::abs(sum-1.0)>1e-12) {
            throw std::invalid_argument("probabilities must sum to 1");
        }

        }
     

double FiniteSet::generate() {
    double u = u_.generate();
    for (std::size_t i = 0; i < cumProbas_.size(); ++i) {
        if (u < cumProbas_[i]) return values_[i];
    }
    return values_.back(); // fallback for rounding
}
