#include "DiscreteGenerator/HeadTail.h"

double HeadTail::generate() {
    return (u_.generate() < 0.5) ? 0.0 : 1.0;
}
