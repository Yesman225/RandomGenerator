#pragma once
#include "RandomProcess.h"

class BrownianND : public RandomProcess
{
    protected:
        std::vector<std::vector<double>>* CorrelationMatrix;

    public:
        BrownianND(RandomGenerator* Gen, int dim, std::vector<std::vector<double>>* Corr);
        void Simulate(double startTime, double endTime, size_t nbSteps) override;
};
