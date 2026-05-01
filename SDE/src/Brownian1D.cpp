#include "Brownian1D.h"
#include "RandomGenerator.h"
#include "SinglePath.h"
#include <cmath>

Brownian1D::Brownian1D(RandomGenerator* Gen) : RandomProcess(Gen, 1)
{
}

void Brownian1D::Simulate(double startTime, double endTime, size_t nbSteps)
{
    double dt = (endTime - startTime) / nbSteps;
    SinglePath* path = new SinglePath(startTime, endTime, nbSteps);
    path->InsertValue(0.0); // Start at 0 for Brownian motion

    
    double currentValue = 0.0;
    for (size_t i = 0; i < nbSteps; ++i)
    {
        double Z = Generator->generate(); // Standard normal random variable
        currentValue += sqrt(dt) * Z;
        path->InsertValue(currentValue);
    }
    Paths[0] = path;
}
