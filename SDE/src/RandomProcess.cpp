#include "RandomProcess.h"


RandomProcess::RandomProcess(RandomGenerator* Gen, int dim) : Generator(Gen), Dimension(dim)
{
    Paths.resize(dim, nullptr);
}

RandomProcess::~RandomProcess() {};

SinglePath* RandomProcess::GetPath(int dimension)
{
    if (dimension < 0 || dimension >= Dimension)
        throw std::out_of_range("Invalid dimension");

    return Paths[dimension];
}

