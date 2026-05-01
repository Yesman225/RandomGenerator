#include "BSEuler1D.h"
#include <cmath>

BSEuler1D::BSEuler1D(RandomGenerator* Gen, double spot, double rate, double vol)
    : BlackScholes1D(Gen, spot, rate, vol)
{
}

void BSEuler1D::Simulate(double startTime, double endTime, size_t nbSteps)
{
    double dt = (endTime - startTime) / nbSteps;
    double currentValue = Spot;
    Paths[0] = new SinglePath(startTime, endTime, nbSteps);
    Paths[0]->InsertValue(currentValue); // Store the initial value

    for (size_t i = 0; i < nbSteps; ++i) {
        double dW = Generator->generate() * sqrt(dt); // Brownian increment
        currentValue = currentValue * (1 + Rate*dt + Vol*dW); // Euler-Maruyama update
        Paths[0]->InsertValue(currentValue); // Store the simulated path
    }
}
