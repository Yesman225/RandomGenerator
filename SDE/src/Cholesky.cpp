#include "Cholesky.h"
#include <cmath>

std::vector<std::vector<double>> cholesky(const std::vector<std::vector<double>>& A)
{
    size_t n = A.size();
    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j <= i; ++j)
        {
            double sum = 0.0;
            for (size_t k = 0; k < j; ++k)
                sum += L[i][k] * L[j][k];

            if (i == j)
                L[i][j] = sqrt(A[i][i] - sum);
            else
                L[i][j] = (A[i][j] - sum) / L[j][j];
        }
    }

    return L;
}
