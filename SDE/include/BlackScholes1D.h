#pragma once
#include "RandomProcess.h"

class BlackScholes1D : public RandomProcess
{
    protected:
        double Spot;
        double Rate;
        double Vol;

    public:
        BlackScholes1D(RandomGenerator* Gen, double spot, double rate, double vol);
        virtual ~BlackScholes1D();
};
