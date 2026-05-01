#include "BSMilstein1D.h"
#include <cmath>

BSMilstein1D::BSMilstein1D(RandomGenerator* Gen, double spot, double rate, double vol)
    : BlackScholes1D(Gen, spot, rate, vol)
{
}

void BSMilstein1D::Simulate(double startTime, double endTime, size_t nbSteps)
{
    double dt = (endTime - startTime) / nbSteps;
    double currentValue = Spot;
    Paths[0] = new SinglePath(startTime, endTime, nbSteps);
    Paths[0]->InsertValue(currentValue); // Store the initial value

    for (size_t i = 0; i < nbSteps; ++i) {
        double dZ = Generator->generate(); // Brownian increment
        currentValue = currentValue * (1 + Rate*dt + Vol*dZ*sqrt(dt) + 0.5*Vol*Vol*(dZ*dZ-1)*dt); // Milstein update
        Paths[0]->InsertValue(currentValue); // Store the simulated path
    }
}
