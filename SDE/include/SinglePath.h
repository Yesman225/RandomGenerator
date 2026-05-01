#pragma once
#include <vector>

class SinglePath
{
    protected:
        std::vector<double> Values;
        double StartTime;
        double EndTime;
        size_t NbSteps;

    public:
        SinglePath(double startTime, double endTime, size_t nbSteps);
        void InsertValue(double val);
        double GetState(double time);
        const std::vector<double>& GetAllValues() const;
};

