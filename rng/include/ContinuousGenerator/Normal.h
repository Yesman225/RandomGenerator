#ifndef NORMAL_H
#define NORMAL_H

#include "ContinuousGenerator.h"
#include "UniformGenerator/UniformGenerator.h"

enum class NormalAlgo{boxMuller, centralLimit, rejection};

class Normal : public ContinuousGenerator{
    public:
        Normal(UniformGenerator& u, NormalAlgo algo):
            u_(u), algo_(algo){}
        double generate() override;

    private:
        double generateBoxMuller();
        double generateCentralLimit();
        double generateRejection();
        UniformGenerator& u_;
        NormalAlgo algo_;
        bool hasSpare_= false;
        double spare_= 0.0;
};

#endif
