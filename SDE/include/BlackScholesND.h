#pragma once
#include "RandomProcess.h"
#include <vector>

class BlackScholesND : public RandomProcess
{
    protected:
        std::vector<double> Spots;
        std::vector<double> Vols;
        std::vector<std::vector<double>> CorrMatrix;
        std::vector<double> Rates;

    public:
        BlackScholesND(RandomGenerator* Gen, int dim, std::vector<double> spots, std::vector<double> vols, std::vector<std::vector<double>> corrMatrix, std::vector<double> rates);
        void Simulate(double startTime, double endTime, size_t nbSteps) override;
        void SimulateAntithetic(double startTime, double endTime, size_t nbSteps);
        SinglePath* GetAntitheticPath(int d);
};
