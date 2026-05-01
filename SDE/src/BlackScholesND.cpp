#include <cmath>
#include "BlackScholesND.h"
#include "Cholesky.h"

BlackScholesND::BlackScholesND(RandomGenerator* Gen, int dim, std::vector<double> spots, std::vector<double> vols, std::vector<std::vector<double>> corrMatrix, std::vector<double> rates)
    : RandomProcess(Gen, dim), Spots(spots), Vols(vols), CorrMatrix(corrMatrix), Rates(rates)
{
}

void BlackScholesND::Simulate(double startTime, double endTime, size_t nbSteps)
{
    double dt = (endTime - startTime) / nbSteps;
    for (int d = 0; d < Dimension; ++d)
    {
        Paths[d] = new SinglePath(startTime, endTime, nbSteps);
        Paths[d]->InsertValue(Spots[d]); // Initial value at time 0
    }

    // Cholesky decomposition of the correlation matrix
    std::vector<std::vector<double>> L = cholesky(CorrMatrix);

    std::vector<double> currentValues(Dimension);
    double sqrt_dt = sqrt(dt);

    for (int d = 0; d < Dimension; ++d)
        currentValues[d] = Spots[d]; // Initialize current values with initial spots
        
    for (size_t step = 1; step <= nbSteps; ++step)
    {
        std::vector<double> Z(Dimension);
        for (int d = 0; d < Dimension; ++d)
            Z[d] = Generator->generate(); // Generate standard normal random variable

        for (int d = 0; d < Dimension; ++d)
        {
            double increment = 0.0;
            for (int k = 0; k < Dimension; ++k)
                increment += L[d][k] * Z[k]; // Correlated increment
            double newValue = currentValues[d] * (1.0 + Rates[d] * dt + Vols[d] * increment * sqrt_dt); // Euler-Maruyama update
            Paths[d]->InsertValue(newValue);
            currentValues[d] = newValue; // Update current value
        }
    }

}

void BlackScholesND::SimulateAntithetic(double startTime, double endTime, size_t nbSteps)
{
    // Resize Paths to hold both the original and antithetic paths
    Paths.resize(2 * Dimension);
    // This method can be implemented similarly to Simulate, but with antithetic variates.
    // For each generated Z, also generate -Z and simulate the paths accordingly.
    double dt = (endTime - startTime) / nbSteps;
    for (int d = 0; d < 2*Dimension; ++d)
    {
        Paths[d] = new SinglePath(startTime, endTime, nbSteps);
        Paths[d]->InsertValue(Spots[d % Dimension]); // Initial value at time 0
    }

    // Cholesky decomposition of the correlation matrix
    std::vector<std::vector<double>> L = cholesky(CorrMatrix);

    std::vector<double> currentValues(Dimension);
    std::vector<double> currentValuesAntithetic(Dimension);
    double sqrt_dt = sqrt(dt);

    for (int d = 0; d < Dimension; ++d){
        currentValues[d] = Spots[d]; // Initialize current values with initial spots
        currentValuesAntithetic[d] = Spots[d]; // Initialize antithetic current values with initial spots
    }

    for (size_t step = 1; step <= nbSteps; ++step)
    {
        std::vector<double> Z(Dimension);
        for (int d = 0; d < Dimension; ++d)
            Z[d] = Generator->generate(); // Generate standard normal random variable

        for (int d = 0; d < Dimension; ++d)
        {
            double increment = 0.0;
            for (int k = 0; k < Dimension; ++k)
                increment += L[d][k] * Z[k]; // Correlated increment
            double newValue = currentValues[d] * (1.0 + Rates[d] * dt + Vols[d] * increment * sqrt_dt); // Euler-Maruyama update
            double newValueAntithetic = currentValuesAntithetic[d] * (1.0 + Rates[d] * dt - Vols[d] * increment * sqrt_dt); // Antithetic update
            Paths[d]->InsertValue(newValue);
            Paths[d + Dimension]->InsertValue(newValueAntithetic); // Antithetic path
            currentValues[d] = newValue; // Update current value
            currentValuesAntithetic[d] = newValueAntithetic; // Update antithetic current value
        }
    }

}


SinglePath* BlackScholesND::GetAntitheticPath(int d)
{
    if (d < 0 || d >= Dimension)
        throw std::out_of_range("Dimension index out of range");
        
    return Paths[d + Dimension]; // Return the antithetic path corresponding to dimension d
}
