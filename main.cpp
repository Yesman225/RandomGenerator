#include <iostream>
#include <LinearCongruential.h>
#include <EcuyerCombined.h>
#include <FiniteSet.h>
#include <Exponential.h>
#include <Normal.h>
#include <Poisson.h>
#include <PDEGrid2D.h>
#include <SinglePath.h>
#include <Brownian1D.h>
#include "BrownianND.h"
#include "BSEuler1D.h"
#include "BSMilstein1D.h"
#include "BlackScholesND.h"

void TestRandom();
void TestPDE();
void TestSinglePath();
void TestBrownian1D();
void TestBrownianND();
void TestBSEuler1D();
double CallPriceBS(double Spot, double Strike, double Maturity, double Rate, double Volatility);
void TestBSMilstein1D();
void TestBlackScholesND();
double BasketPayoff(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, int nbSteps);
std::vector<double> BasketMCPrice(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, double rate, int nbPaths, int nbSteps);
void TestBlackScholesNDTwoAssets();
void TestBlackScholesNDOStats();
void TestBlackScholesNDAntithetic();
std::vector<double> BasketMCPriceAntithetic(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, double rate, int nbPaths, int nbSteps);
std::vector<double> BasketPayoffAntithetic(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, int nbSteps);


int main()
{
    TestBlackScholesNDAntithetic();
}

void TestRandom()
{
    mylong Multiplier1, Increment1, Modulus1, Seed1, Multiplier2, Increment2, Modulus2, Seed2 ;

    Multiplier1 = 40014;
    Increment1 = 0;
    Modulus1 = 2147483563;
    Seed1 = 1;
    Multiplier2 = 40692;
    Increment2 = 0;
    Modulus2 = 2147483399;
    Seed2 = 1;

    //X = new LinearCongruential(Multiplier, Increment, Modulus, Seed);
    //X = new EcuyerCombined();
    UniformGenerator* Unif = new EcuyerCombined(Seed1, Multiplier1, Modulus1, Seed2, Multiplier2, Modulus2);
    std::vector<double> probs(3);
    probs[0] = 0.4;
    probs[1] = 0.5;
    probs[2] = 0.1;
    RandomGenerator* Finit = new FiniteSet(Unif, probs);
    /*
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << Finit->Generate() << std::endl;
        }

        ExponentialInverseDistribution* Exp = new ExponentialInverseDistribution(0.5, Unif);
        */
    //bool isAverageOk, isVarianceOk;
    //isAverageOk = Exp->TestMean(100000, 0.01);
    //isVarianceOk = Exp->TestVariance(100000, 0.1);

    Normal NormBox(*Unif, NormalAlgo::boxMuller);
    NormBox.moments(100000);
}

void TestPDE()
{
    /*
        Evaluate a Call option in the Black-Scholes framework:
        Expected Price : 6.80495771
    */
    double Spot = 100;
    double Strike = 100;
    double Maturity = 1;
    double Rate = 0.05;
    double Volatility = 0.1;

    double SMin = 0;
    double SMax = 1000;

    vector <double> Theta = {0.0, 0.5, 1.0};

    R2R1Function* VarianceFunction = new BSVariance(Volatility);
    R2R1Function* TrendFunction = new BSTrend(Rate);
    R2R1Function* ActualizationFunction = new BSActualization(Rate);
    R2R1Function* SourceTermFunction = new NullFunction();

    R1R1Function* TopBoundaryFunction = new CallTopBoundary(SMax, Strike, Rate);
    R1R1Function* BottomBoundaryFunction = new CallBottomBoundary(SMin, Strike);
    R1R1Function* RightBoundaryFunction = new CallTerminalCondition(Strike);

    PDEGrid2DExplicit BlackScholesGridEx(Maturity, SMin, SMax, 10000, 1.,
        VarianceFunction, TrendFunction, ActualizationFunction, SourceTermFunction,
        TopBoundaryFunction, BottomBoundaryFunction, RightBoundaryFunction);
    
    PDEGrid2DImplicit BlackScholesGridIm(Maturity, SMin, SMax, 10000, 1.,
        VarianceFunction, TrendFunction, ActualizationFunction, SourceTermFunction,
        TopBoundaryFunction, BottomBoundaryFunction, RightBoundaryFunction);
    
  
    BlackScholesGridEx.FillNodes();
    BlackScholesGridIm.FillNodes();
    double priceAtZeroEx = BlackScholesGridEx.GetTimeZeroNodeValue(Spot);
    double priceAtZeroIm = BlackScholesGridIm.GetTimeZeroNodeValue(Spot);

    std::cout << "Price = " << "With Explicit Scheme : "<< priceAtZeroEx << std::endl;
    std::cout << "Price = " << "With Implicit Scheme : "<< priceAtZeroIm << std::endl;
       
    for (size_t i = 0; i<Theta.size(); ++i)
    {

        PDEGridTheta BlackScholesGridTheta (Maturity, SMin, SMax, 10000, 1.,
        VarianceFunction, TrendFunction, ActualizationFunction, SourceTermFunction,
        TopBoundaryFunction, BottomBoundaryFunction, RightBoundaryFunction, Theta[i]);

        BlackScholesGridTheta.FillNodes();
        double priceAtZeroTheta = BlackScholesGridTheta.GetTimeZeroNodeValue(Spot);
    
        std::cout << "Price = " << "With Theta(="<< Theta[i]<<"): "<< priceAtZeroTheta << std::endl;
    }

}

void TestSinglePath()
{
   SinglePath path(0.0, 1.0, 4);
   path.InsertValue(10.0);
   path.InsertValue(20.0);
   path.InsertValue(30.0);
   path.InsertValue(40.0);
   path.InsertValue(50.0);

   std::vector<double> allValues = path.GetAllValues();
    for (size_t i = 0; i < allValues.size(); ++i)
    {
        std::cout << "Value at index " << i << ": " << allValues[i] << std::endl;
    }

    std::cout << "Value at time 0: " << path.GetState(0.0) << std::endl;
    std::cout << "Value at time 0.5: " << path.GetState(0.5) << std::endl;

    std::cout << "Value at time 0.37 " << path.GetState(0.37) << std::endl;
    try {
    double val = path.GetState(-0.1);
    std::cout << "Value: " << val << std::endl;
    } catch (const std::out_of_range& e) {
    std::cout << "Caught exception: " << e.what() << std::endl;
    }
}
void TestBrownian1D()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    Brownian1D* Brown = new Brownian1D(Norm);

    double sum = 0.0;
    double sumSq = 0.0;
    for (size_t i = 0; i < 100000; ++i)
    {
        Brown->Simulate(0.0, 1.0, 100);
        sum += Brown->GetPath(0)->GetState(1.0);
        sumSq += Brown->GetPath(0)->GetState(1.0) * Brown->GetPath(0)->GetState(1.0);
    }

    double mean = sum / 100000.0;
    double variance = (sumSq / 100000.0) - (mean * mean);
    std::cout << "Brownian Motion 1D - Mean: " << mean << ", Variance: " << variance << std::endl;
}

void TestBrownianND()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    
    int dimension = 2;
    
    std::vector<std::vector<double>> correlationMatrix = {
        {1.0, 0.6},
        {0.6, 1.0}
    };
    
    BrownianND* BrownND = new BrownianND(Norm, dimension, &correlationMatrix);

    vector<double> sum(dimension, 0.0);
    vector<double> sumSq(dimension, 0.0);
    vector<double> values(dimension, 0.0);
    double sumCross = 0.0; // To calculate the cross product for correlation

    for (size_t i = 0; i < 100000; ++i)
    {
        BrownND->Simulate(0.0, 1.0, 100);
        for (int d = 0; d < dimension; ++d)
        {
            double value = BrownND->GetPath(d)->GetState(1.0);
            values[d] = value;
            sum[d] += value;
            sumSq[d] += value * value;
        }
            sumCross += values[0] * values[1]; // Accumulate the product for correlation
    }

    vector<double> mean(dimension);
    vector<double> variance(dimension);

    for (int d = 0; d < dimension; ++d)
    {
        mean[d] = sum[d] / 100000.0;
        variance[d] = (sumSq[d] / 100000.0) - (mean[d] * mean[d]);
        std::cout << "Brownian Motion " << d + 1 << "D - Mean: " << mean[d] << ", Variance: " << variance[d] << std::endl;
    }

    double correlation = (sumCross / 100000.0 - mean[0] * mean[1]) / (sqrt(variance[0]) * sqrt(variance[1]));
    std::cout << "Correlation between the two dimensions: " << correlation << std::endl;
}

double CallPriceBS(double Spot, double Strike, double Maturity, double Rate, double Volatility)
{
    // Black-Scholes formula for a European call option - helper function to compare with PDE results
    double d1 = (log(Spot / Strike) + (Rate + 0.5 * Volatility * Volatility) * Maturity) / (Volatility * sqrt(Maturity));
    double d2 = d1 - Volatility * sqrt(Maturity);
    double callPrice = Spot * 0.5 * (1.0 + erf(d1 / sqrt(2))) - Strike * exp(-Rate * Maturity) * 0.5 * (1.0 + erf(d2 / sqrt(2)));
    return callPrice;
}

void TestBSEuler1D()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    BSEuler1D* BSEuler = new BSEuler1D(Norm, 100.0, 0.05, 0.2);
    double strike = 100.0;
    double payoffSum = 0.0;

    for (size_t i = 0; i < 100000; ++i)
    {
        BSEuler->Simulate(0.0, 1.0, 252);
        double finalValue = BSEuler->GetPath(0)->GetState(1.0);
        payoffSum += std::max(finalValue - strike, 0.0); // Call option payoff
    }

    double callPrice = exp(-0.05) * (payoffSum / 100000.0); // Average payoff
    std::cout << "Call option price (BSEuler1D): " << callPrice << std::endl;
    std::cout << "Call option price (Black-Scholes formula): " << CallPriceBS(100.0, strike, 1.0, 0.05, 0.2) << std::endl;
}

void TestBSMilstein1D()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    BSMilstein1D* BSMilstein = new BSMilstein1D(Norm, 100.0, 0.05, 0.2);
    BSEuler1D* BSEuler = new BSEuler1D(Norm, 100.0, 0.05, 0.2);

    double strike = 100.0;
    double payoffSum_milstein = 0.0;
    double payoffSum_euler = 0.0;

    for (size_t i = 0; i < 100000; ++i)
    {
        BSMilstein->Simulate(0.0, 1.0, 10);
        BSEuler->Simulate(0.0, 1.0, 10);
        double finalValueMilstein = BSMilstein->GetPath(0)->GetState(1.0);
        double finalValueEuler = BSEuler->GetPath(0)->GetState(1.0);
        payoffSum_milstein += std::max(finalValueMilstein - strike, 0.0); // Call option payoff
        payoffSum_euler += std::max(finalValueEuler - strike, 0.0); // Call option payoff
    }

    double callPriceMilstein = exp(-0.05) * (payoffSum_milstein / 100000.0); // Average payoff
    double callPriceEuler = exp(-0.05) * (payoffSum_euler / 100000.0); // Average payoff
    std::cout << "Call option price (BSMilstein1D): " << callPriceMilstein << std::endl;
    std::cout << "Call option price (BSEuler1D): " << callPriceEuler << std::endl;
    std::cout << "Call option price (Black-Scholes formula): " << CallPriceBS(100.0, strike, 1.0, 0.05, 0.2) << std::endl;
}

double BasketPayoff(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, int nbSteps)
{
    double payoff = 0.0;
    process->Simulate(0.0, maturity, nbSteps); // Simulate the process up to maturity
    for (size_t d = 0; d < weights.size(); ++d)
    {
        payoff += weights[d] * process->GetPath(d)->GetState(maturity);
    }
    return std::max(payoff - strike, 0.0); // Call option payoff
}

std::vector<double> BasketMCPrice(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, double rate, int nbPaths, int nbSteps)
{
    double payoffSum = 0.0;
    double payoffSumSq = 0.0;
    for (int i = 0; i < nbPaths; ++i)
    {
        double payoff = BasketPayoff(process, weights, strike, maturity, nbSteps);
        payoffSum += payoff;
        payoffSumSq += payoff * payoff;
    }
    double variancePrice = (payoffSumSq / nbPaths - (payoffSum / nbPaths) * (payoffSum / nbPaths)); // Variance
    return {exp(-rate * maturity) * (payoffSum / nbPaths), variancePrice};
}

void TestBlackScholesND()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    
    int dimension = 1;

    std::vector<double> spots = {100.0};
    std::vector<double> vols = {0.2};
    std::vector<std::vector<double>> correlationMatrix = {
        {1.0}
    };
    std::vector<double> rates = {0.05};
    BlackScholesND* BSND = new BlackScholesND(Norm, dimension, spots, vols, correlationMatrix, rates);
    std::vector<double> weights = {1.0};

    double strike = 100.0;
    double maturity = 1.0;
    double price = BasketMCPrice(BSND, weights, strike, maturity, rates[0], 100000, 252)[0];
    std::cout << "Basket option price (Black-Scholes ND): " << price << std::endl;
    std::cout << "Call option price (Black-Scholes formula): " << CallPriceBS(100.0, strike, maturity, rates[0], vols[0]) << std::endl;
}

void TestBlackScholesNDTwoAssets()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    
    int dimension = 2;

    std::vector<double> spots = {100.0, 100.0};
    std::vector<double> vols = {0.2, 0.2};
    std::vector<std::vector<double>> correlationMatrix = {
        {1.0, 0.6},
        {0.6, 1.0}
    };
    std::vector<double> rates = {0.05, 0.05};
    BlackScholesND* BSND = new BlackScholesND(Norm, dimension, spots, vols, correlationMatrix, rates);
    std::vector<double> weights = {1.0, 0.0};

    double strike = 100.0;
    double maturity = 1.0;
    double price = BasketMCPrice(BSND, weights, strike, maturity, rates[0], 100000, 252)[0];
    std::cout << "Basket option price (Black-Scholes ND Two Assets): " << price << std::endl;
    std::cout << "Call option price (Black-Scholes formula): " << CallPriceBS(100.0, strike, maturity, rates[0], vols[0]) << std::endl;
}

void TestBlackScholesNDOStats()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);

    int dimension = 2;

    std::vector<double> spots = {100.0, 100.0};
    std::vector<double> vols = {0.2, 0.2};
    std::vector<std::vector<double>> correlationMatrix = {
        {1.0, 0.6},
        {0.6, 1.0}
    };
    std::vector<double> rates = {0.05, 0.05};
    BlackScholesND* BSND = new BlackScholesND(Norm, dimension, spots, vols, correlationMatrix, rates);
    std::vector<double> weights = {0.5, 0.5};

    double strike = 100.0;
    double maturity = 1.0;
    int nbPaths = 100000;
    vector<double> finalValues(dimension, 0.0);
    vector<double> sum(dimension, 0.0);
    vector<double> sumSq(dimension, 0.0);

    vector<double> logReturns(dimension, 0.0);
    vector<double> sum_logRet(dimension, 0.0);
    vector<double> sumSq_logRet(dimension, 0.0);
    double sumCross = 0.0;

    for (int i = 0; i < nbPaths; ++i)
    {
        BSND->Simulate(0.0, maturity, 252);
        finalValues[0] = BSND->GetPath(0)->GetState(maturity);
        finalValues[1] = BSND->GetPath(1)->GetState(maturity);
        sum[0] += finalValues[0];
        sum[1] += finalValues[1];
        sumSq[0] += finalValues[0] * finalValues[0];
        sumSq[1] += finalValues[1] * finalValues[1];

        logReturns[0] = std::log(finalValues[0]) / spots[0]; // Log return for asset 1
        logReturns[1] = std::log(finalValues[1]) / spots[1]; // Log return for asset 2
        sum_logRet[0] += logReturns[0];
        sum_logRet[1] += logReturns[1];
        sumSq_logRet[0] += logReturns[0] * logReturns[0];
        sumSq_logRet[1] += logReturns[1] * logReturns[1];
        sumCross += logReturns[0] * logReturns[1];
    }

    vector<double> mean(dimension);
    vector<double> variance(dimension);
    for (int i = 0; i < dimension; ++i)
    {
        mean[i] = sum[i] / nbPaths;
        variance[i] = (sumSq[i] / nbPaths) - (mean[i] * mean[i]);
    }

    std::cout << "Asset 1 - Mean: " << mean[0] << ", Variance: " << variance[0] << std::endl;
    std::cout << "Asset 2 - Mean: " << mean[1] << ", Variance: " << variance[1] << std::endl;

    // Correlation of log returns
    double mean_logRet_0 = sum_logRet[0] / nbPaths;
    double mean_logRet_1 = sum_logRet[1] / nbPaths;
    double var_logRet_0 = sumSq_logRet[0] / nbPaths - mean_logRet_0 * mean_logRet_0;
    double var_logRet_1 = sumSq_logRet[1] / nbPaths - mean_logRet_1 * mean_logRet_1;
    double correlation = (sumCross / nbPaths - mean_logRet_0 * mean_logRet_1) / (sqrt(var_logRet_0) * sqrt(var_logRet_1));
    std::cout << "Correlation between the two assets: " << correlation << std::endl;
}

std::vector<double> BasketPayoffAntithetic(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, int nbSteps)
{
    double payoff = 0.0;
    double payoffAntithetic = 0.0;

    process->SimulateAntithetic(0.0, maturity, nbSteps); // Simulate the process up to maturity with antithetic variates
    for (size_t d = 0; d < weights.size(); ++d)
    {
        payoff += weights[d] * process->GetPath(d)->GetState(maturity);
        payoffAntithetic += weights[d] * process->GetAntitheticPath(d)->GetState(maturity);
    }
    payoff = std::max(payoff - strike, 0.0); // Call option payoff
    payoffAntithetic = std::max(payoffAntithetic - strike, 0.0); // Call option payoff for antithetic path

    return {payoff, payoffAntithetic};
}

std::vector<double> BasketMCPriceAntithetic(BlackScholesND* process, std::vector<double> weights, double strike, double maturity, double rate, int nbPaths, int nbSteps)
{
    double payoffSum = 0.0;
    double payoffAntitheticSum = 0.0;
    double payoffSumSq = 0.0;
    double payoffAntitheticSumSq = 0.0;

    for (int i = 0; i < nbPaths; ++i)
    {
        std::vector<double> payoffs = BasketPayoffAntithetic(process, weights, strike, maturity, nbSteps);
          /* if (i < 10)
            std::cout << "payoff: " << payoffs[0] << "  antithetic: " << payoffs[1] << std::endl;
            std::cout << "payoff: " << payoffs[0] << "  antithetic: " << payoffs[1] << std::endl;

        */
        payoffSum += payoffs[0]; // Original path
        payoffAntitheticSum += (payoffs[0] + payoffs[1]) / 2; // Antithetic path
        payoffSumSq += payoffs[0] * payoffs[0]; // Original path
        payoffAntitheticSumSq += (payoffs[0] + payoffs[1]) / 2 * (payoffs[0] + payoffs[1]) / 2; // Antithetic path
    }
    double price = exp(-rate * maturity) * (payoffSum + payoffAntitheticSum )/ (2.0 * nbPaths); // Average payoff discounted
    double variancePriceStandard = (payoffSumSq / nbPaths - (payoffSum / nbPaths) * (payoffSum / nbPaths)); // Variance of the original path
    double variancePriceAntithetic = (payoffAntitheticSumSq / nbPaths - (payoffAntitheticSum / nbPaths) * (payoffAntitheticSum / nbPaths)); // Variance of the antithetic path

 

    return {price, variancePriceStandard, variancePriceAntithetic};
}

void TestBlackScholesNDAntithetic()
{
    UniformGenerator* Unif = new EcuyerCombined(1, 40014, 2147483563, 1, 40692, 2147483399);
    Normal* Norm = new Normal(*Unif, NormalAlgo::boxMuller);
    
    int dimension = 2;

    std::vector<double> spots = {100.0, 100.0};
    std::vector<double> vols = {0.2, 0.2};
    std::vector<std::vector<double>> correlationMatrix = {
        {1.0, 0.6},
        {0.6, 1.0}
    };
    std::vector<double> rates = {0.05, 0.05};
    BlackScholesND* BSND = new BlackScholesND(Norm, dimension, spots, vols, correlationMatrix, rates);
    std::vector<double> weights = {1.0, 0.0};

    double strike = 100.0;
    double maturity = 1.0;
    std::vector<double> results = BasketMCPrice(BSND, weights, strike, maturity, rates[0], 100000, 252);
    std::vector<double> results_Antithetic = BasketMCPriceAntithetic(BSND, weights, strike, maturity, rates[0], 100000, 252);
    double priceStandard = results[0];
    double priceAntithetic = results_Antithetic[0];
    double variancePriceStandard = results[1];
    double variancePriceAntithetic = results_Antithetic[2];
    std::cout << "Basket option price (Black-Scholes ND Standard): " << priceStandard << std::endl;
    std::cout << "Basket option price (Black-Scholes ND Antithetic): " << priceAntithetic << std::endl;
    std::cout << "Variance of the price (Standard MC): " << variancePriceStandard << std::endl;
    std::cout << "Variance of the price (Antithetic MC): " << variancePriceAntithetic << std::endl;
    std::cout << "Call option price (Black-Scholes formula): " << CallPriceBS(100.0, strike, maturity, rates[0], vols[0]) << std::endl;
}
