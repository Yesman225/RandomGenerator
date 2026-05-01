#pragma once
#include "RandomGenerator.h"
#include "SinglePath.h"
#include <stdexcept>

class RandomProcess
{
protected:
    RandomGenerator* Generator;
    std::vector<SinglePath*> Paths;
    int Dimension;

public:
    RandomProcess(RandomGenerator* Gen, int dim);
    virtual ~RandomProcess();
    virtual void Simulate(double startTime, double endTime, size_t nbSteps) = 0;
    SinglePath* GetPath(int dimension = 0);
};
