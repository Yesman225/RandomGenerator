#include "PDEGrid2D.h"
#include <stdexcept>

PDEGrid2D::PDEGrid2D(
	double Maturity,
	double MinUnderlyingValue,
	double MaxUnderlyingValue,
	int NbTimeSteps,
	double StepForUnderlying,
	R2R1Function* VarianceFunction,
	R2R1Function* TrendFunction,
	R2R1Function* ActualizationFunction,
	R2R1Function* SourceTermFunction,
	R1R1Function* TopBoundaryFunction,
	R1R1Function* BottomBoundaryFunction,
	R1R1Function* RightBoundaryFunction
) :
	T(Maturity),
	MinX(MinUnderlyingValue),
	MaxX(MaxUnderlyingValue),
	h0(Maturity / NbTimeSteps),
	h1(StepForUnderlying),
	a(VarianceFunction),
	b(TrendFunction),
	r(ActualizationFunction),
	f(SourceTermFunction),
	TopBoundaryFunction(TopBoundaryFunction),
	BottomBoundaryFunction(BottomBoundaryFunction),
	RightBoundaryFunction(RightBoundaryFunction)
{
	NodesHeight = (MaxX - MinX) / h1 + 1;
	NodesWidth = NbTimeSteps + 1;
	Nodes = vector< vector<double> >(NodesWidth);
	for (size_t i = 0; i < NodesWidth; ++i)
	{
		Nodes[i] = vector<double>(NodesHeight);
	}
}

PDEGrid2D::~PDEGrid2D()
{
}

void PDEGrid2D::FillRightBoundary()
{
	for (size_t j = 0; j < NodesHeight; ++j)
	{
		//Nodes[NodesWidth - 1][j] = RightBoundaryFunction->operator()(j * h1);
		Nodes[NodesWidth - 1][j] = (*RightBoundaryFunction)(MinX + j * h1);
	}
}

void PDEGrid2D::FillTopAndBottomBoundary()
{
	for (size_t i = 0; i < NodesWidth; ++i)
	{
		Nodes[i][0] = (*BottomBoundaryFunction)(i * h0);
		Nodes[i][NodesHeight - 1] = (*TopBoundaryFunction)(i * h0);
	}
}

void PDEGrid2D::FillNodes()
{
	FillRightBoundary();
	FillTopAndBottomBoundary();
}

double PDEGrid2D::GetTimeZeroNodeValue(double spot)
{
	return Nodes[0][(int)((spot - MinX) / h1)];
}

PDEGrid2DExplicit::PDEGrid2DExplicit(
	double Maturity,
	double MinUnderlyingValue,
	double MaxUnderlyingValue,
	int NbTimeSteps,
	double StepForUnderlying,
	R2R1Function* VarianceFunction,
	R2R1Function* TrendFunction,
	R2R1Function* ActualizationFunction,
	R2R1Function* SourceTermFunction,
	R1R1Function* TopBoundaryFunction,
	R1R1Function* BottomBoundaryFunction,
	R1R1Function* RightBoundaryFunction
) : PDEGrid2D(
	Maturity,
	MinUnderlyingValue,
	MaxUnderlyingValue,
	NbTimeSteps,
	StepForUnderlying,
	VarianceFunction,
	TrendFunction,
	ActualizationFunction,
	SourceTermFunction,
	TopBoundaryFunction,
	BottomBoundaryFunction,
	RightBoundaryFunction
)
{}

PDEGrid2DImplicit::PDEGrid2DImplicit(
	double Maturity,
	double MinUnderlyingValue,
	double MaxUnderlyingValue,
	int NbTimeSteps,
	double StepForUnderlying,
	R2R1Function* VarianceFunction,
	R2R1Function* TrendFunction,
	R2R1Function* ActualizationFunction,
	R2R1Function* SourceTermFunction,
	R1R1Function* TopBoundaryFunction,
	R1R1Function* BottomBoundaryFunction,
	R1R1Function* RightBoundaryFunction
) : PDEGrid2D(
	Maturity,
	MinUnderlyingValue,
	MaxUnderlyingValue,
	NbTimeSteps,
	StepForUnderlying,
	VarianceFunction,
	TrendFunction,
	ActualizationFunction,
	SourceTermFunction,
	TopBoundaryFunction,
	BottomBoundaryFunction,
	RightBoundaryFunction
)
{}

void PDEGrid2DExplicit::FillNodes()
{
	PDEGrid2D::FillNodes();

	for (size_t k = (NodesWidth - 1); k > 0; --k)
	{
		for (size_t j = 1; j < NodesHeight - 1; ++j)
		{
			double x = MinX + j * h1;
			double t = k * h0;

			double AjkH0ToH1Square = h0 * (*a)(x, t) / (h1 * h1);
			double BjkH0ToH1 = h0 * (*b)(x, t) / h1;

			if ((k == (NodesWidth - 1)) && j == (NodesHeight-2))
			{
				double Vjp1k = Nodes[k][j + 1];
				double Vjk = Nodes[k][j];
				double vjm1k = Nodes[k][j - 1];
			}
			Nodes[k - 1][j] = Nodes[k][j] * (1 - AjkH0ToH1Square - BjkH0ToH1 - h0 * (*r)(x, t))
							+ Nodes[k][j + 1] * (BjkH0ToH1 + 0.5*AjkH0ToH1Square)
							+ Nodes[k][j - 1] * (0.5 * AjkH0ToH1Square)
							+ h0 * (*f)(x, t);
		}
	}
}

void PDEGrid2DImplicit::FillNodes()
{
	PDEGrid2D::FillNodes();

	const int N = NodesHeight - 2;

	//Tri-diagonal system vectors
	vector<double> lower(N), diag(N), upper(N), rhs(N), sol(N);

	for (size_t k = (NodesWidth - 1); k > 0; --k)
	{
		const double t_new = (k-1)*h0;

		// Build the tridiagonal system -------------------
		for (int idx = 0; idx < N; ++idx)
		{
			const int j = idx + 1;
			const double x = MinX + j*h1;

			const double A = h0 * (*a)(x, t_new) / (h1 * h1);
			const double B = h0 * (*b)(x, t_new) / h1;
			const double R = h0 * (*r)(x, t_new);

			lower[idx] = -0.5 * A;
			diag[idx] = 1.0 + A + B + R;
			upper[idx] = -(0.5 * A + B);

			rhs[idx] = Nodes[k][j] + h0 * (*f)(x,t_new);
		}	
		rhs[0] -= lower[0] * Nodes[k - 1][0]; // Bottom boundary
		rhs[N-1] -= upper[N-1] * Nodes[k-1][NodesHeight -1]; // Top Boundary

		// Thomas Algorithm - forward sweep ------------------
		for (int i = 1; i <N; ++i)
		{
			const double factor = lower[i] / diag[i - 1];
            diag[i] -= factor * upper[i - 1];
            rhs [i] -= factor * rhs  [i - 1];
		}

		 // ── Thomas algorithm – back substitution ─────────────────────────
        sol[N - 1] = rhs[N - 1] / diag[N - 1];
        for (int i = N - 2; i >= 0; --i)
        {
            sol[i] = (rhs[i] - upper[i] * sol[i + 1]) / diag[i];
        }

        // ── Write interior nodes at time level k-1 ───────────────────────
        for (int idx = 0; idx < N; ++idx)
        {
            Nodes[k - 1][idx + 1] = sol[idx];
        }
	}
}

PDEGridTheta::PDEGridTheta(
	double Maturity,
	double MinUnderlyingValue,
	double MaxUnderlyingValue,
	int NbTimeSteps,
	double StepForUnderlying,
	R2R1Function* VarianceFunction,
	R2R1Function* TrendFunction,
	R2R1Function* ActualizationFunction,
	R2R1Function* SourceTermFunction,
	R1R1Function* TopBoundaryFunction,
	R1R1Function* BottomBoundaryFunction,
	R1R1Function* RightBoundaryFunction,
	double theta
) : PDEGrid2D(
	Maturity,
	MinUnderlyingValue,
	MaxUnderlyingValue,
	NbTimeSteps,
	StepForUnderlying,
	VarianceFunction,
	TrendFunction,
	ActualizationFunction,
	SourceTermFunction,
	TopBoundaryFunction,
	BottomBoundaryFunction,
	RightBoundaryFunction
), theta_(theta)
{
	if (theta < 0.0 || theta > 1.0) throw std::invalid_argument("Theta should be in range [0.0, 1.0]");
}

void PDEGridTheta::FillNodes()
{
	PDEGrid2D::FillNodes();

	const int N = NodesHeight - 2;

	//Tri-diagonal system vectors
	vector<double> lower(N), diag(N), upper(N), rhs(N), sol(N);

	for (size_t k = (NodesWidth - 1); k > 0; --k)
	{
		const double t_new = (k-1)*h0;

		// Build the tridiagonal system -------------------
		for (int idx = 0; idx < N; ++idx)
		{
			const int j = idx + 1;
			const double x = MinX + j*h1;

			const double A = h0 * (*a)(x, t_new) / (h1 * h1);
			const double B = h0 * (*b)(x, t_new) / h1;
			const double R = h0 * (*r)(x, t_new);

			lower[idx] = - theta_ * 0.5 * A;
			diag[idx] = 1.0 +theta_ * (A + B + R);
			upper[idx] = -theta_ * (0.5 * A + B);

			rhs[idx] = (1 - theta_) * ((1 - A - B - R) * Nodes[k][j] 
                          + (B + 0.5*A) * Nodes[k][j+1] 
                          + 0.5*A * Nodes[k][j-1] 
                          + h0 * (*f)(x, t_new))
         + theta_ * (Nodes[k][j] + h0 * (*f)(x, t_new));
		}	
		rhs[0] -= lower[0] * Nodes[k-1][0]; // Bottom boundary
		rhs[N-1] -= upper[N-1] * Nodes[k-1][NodesHeight -1]; // Top Boundary

		// Thomas Algorithm - forward sweep ------------------
		for (int i = 1; i <N; ++i)
		{
			const double factor = lower[i] / diag[i - 1];
            diag[i] -= factor * upper[i - 1];
            rhs [i] -= factor * rhs  [i - 1];
		}

		 // ── Thomas algorithm – back substitution ─────────────────────────
        sol[N - 1] = rhs[N - 1] / diag[N - 1];
        for (int i = N - 2; i >= 0; --i)
        {
            sol[i] = (rhs[i] - upper[i] * sol[i + 1]) / diag[i];
        }

        // ── Write interior nodes at time level k-1 ───────────────────────
        for (int idx = 0; idx < N; ++idx)
        {
            Nodes[k - 1][idx + 1] = sol[idx];
        }
	}
}
