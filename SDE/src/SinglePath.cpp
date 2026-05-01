#include "SinglePath.h"
#include <stdexcept>
#include <cmath>

SinglePath::SinglePath(double startTime, double endTime, size_t nbSteps) :
    StartTime(startTime), EndTime(endTime), NbSteps(nbSteps)
{
}

void SinglePath::InsertValue(double val)
{
    Values.push_back(val);
}

double SinglePath::GetState(double time)
{
    if (time < StartTime || time > EndTime)
        throw std::out_of_range("Time is out of range");

    size_t index = static_cast<size_t>(std::floor((time - StartTime) / (EndTime - StartTime) * NbSteps));

    if (index >= Values.size())
        throw std::out_of_range("Index is out of range");

    return Values[index];
}

const std::vector<double>& SinglePath::GetAllValues() const
{
    return Values;
}
