#include "BrownianND.h"
#include "Cholesky.h"

using namespace std;

BrownianND::BrownianND(RandomGenerator* Gen, int dim, std::vector<std::vector<double>>* Corr)
    : RandomProcess(Gen, dim), CorrelationMatrix(Corr)
{
}

void BrownianND::Simulate(double startTime, double endTime, size_t nbSteps)
{
    double dt = (endTime - startTime) / nbSteps;
    vector<vector<double>> L = cholesky(*CorrelationMatrix);

    for (int d = 0; d < Dimension; ++d)
    {
        Paths[d] = new SinglePath(startTime, endTime, nbSteps);
        Paths[d]->InsertValue(0.0); // Initial value at time 0
    }

    vector<double> currentValues(Dimension, 0.0); // To store current values of the paths
    double sqrt_dt = sqrt(dt);

    for (size_t step = 1; step <= nbSteps; ++step)
    {
        vector<double> Z(Dimension);
        for (int d = 0; d < Dimension; ++d)
            Z[d] = Generator->generate(); // Generate standard normal random variable

        for (int d = 0; d < Dimension; ++d)
        {
            double increment = 0.0;
            for (int k = 0; k < Dimension; ++k)
                increment += L[d][k] * Z[k]; // Correlated increment
            currentValues[d] += increment * sqrt_dt; // Update current value
            Paths[d]->InsertValue(currentValues[d]);
        }
    }
}
