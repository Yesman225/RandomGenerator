#include "ContinuousGenerator/Normal.h"
#include <stdexcept>
#include <cmath>
#include <limits>

double Normal::generate(){
    if (algo_ == NormalAlgo::rejection) {
        return Normal::generateRejection();
    }
    if (algo_ == NormalAlgo::boxMuller){
        return Normal::generateBoxMuller();
    }

    return Normal::generateCentralLimit();
}

double Normal::generateBoxMuller(){
    if (hasSpare_) {
            hasSpare_=false;
            return spare_;
        }

    double u1= u_.generate();
    double u2= u_.generate();
    
    if (u1==0.0) u1=std::numeric_limits<double>::min();

    const double r = std::sqrt(-2.0 * std::log(u1));
    const double theta = 2.0 * 3.14159265358979323846 * u2;

    spare_ = r*std::sin(theta);
    hasSpare_ = true;
    return r*std::cos(theta);
}

double Normal::generateCentralLimit(){
    double s = 0.0;
    for (int i = 0; i<12; ++i){
        s+= u_.generate();
    }

    return s - 6.0;

}

double Normal::generateRejection(){
    
    for(;;){
        const double u1 = u_.generate();  // Uniform in (0,1)
        const double y = -std::log(u1);   // Exp(1)
        
        const double u2 = u_.generate();
        const double sgn = (u2<0.5) ? 1.0 : -1.0;

        const double x = sgn * y;

        // Acceptance probability
        const double a = std::exp(-0.5 * (y - 1.0) * (y - 1.0));

        // Accept / Reject
        const double u3 = u_.generate();
        if (u3 <= a) return x;
    }
}

