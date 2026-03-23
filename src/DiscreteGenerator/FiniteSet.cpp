#include "DiscreteGenerator/FiniteSet.h"
#include <cmath>
#include <stdexcept>
#include <cstddef>


FiniteSet::FiniteSet(UniformGenerator* u, std::vector<double>& probas):
        FiniteSet(*u, probas) {}

FiniteSet::FiniteSet(UniformGenerator& u, std::vector<double>& probas): 
        u_(u), probas_(probas) {

        double sum = 0.0;
        
        for (double p : probas_) {
            if (p<0.0 || p>1.0) throw std::invalid_argument("probability should be in [0,1]");
            sum += p;
        }
        
        if(std::abs(sum-1.0)>1e-12) {
            throw std::invalid_argument("probabilities must sum to 1");
        }

        }
     

double FiniteSet::generate() {
    double u = u_.generate();
    double result = 0.;
	double Pkminus1 = 0;
	double Pk = 0;

	mylong K = 1;

	for (mylong i = 0; i < probas_.size(); ++i)
	{
		Pk += probas_[i];
		if (Pkminus1 <= u && u < Pk)
		{
			result = K;
			break;
		}
		else
		{
			Pkminus1 = Pk;
		}
		++K;
	}
	result = K;
	return result;
}
