#ifndef HEADTAIL_H
#define HEADTAIL_H

#include "DiscreteGenerator.h"
#include "UniformGenerator/UniformGenerator.h"


class HeadTail : public DiscreteGenerator {
    public:
        explicit HeadTail(UniformGenerator& u): u_(u){}
        double generate() override;
    
    private:
        UniformGenerator& u_;
};
#endif
