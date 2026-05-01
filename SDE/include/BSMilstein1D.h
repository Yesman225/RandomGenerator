
#pragma once
#include "BlackScholes1D.h"

class BSMilstein1D : public BlackScholes1D
{
    public:
        BSMilstein1D(RandomGenerator* Gen, double spot, double rate, double vol);
        void Simulate(double startTime, double endTime, size_t nbSteps) override;
};
