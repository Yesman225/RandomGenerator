#include "UniformGenerator/LinearCongruential.h"
#include <stdexcept>
#include <cmath>

double LinearCongruential::generate() {
    if (modulus_<=0) {throw std::invalid_argument("modulus must be >0");}
    seed_ = std::fmod(multiplier_ * seed_ + increment_, modulus_);
    if (seed_<0.0) seed_+=modulus_;
    return seed_ / modulus_;
}

double LinearCongruential::generateRaw() {
    if (modulus_<=0) {throw std::invalid_argument("modulus must be >0");}
    seed_ = std::fmod(multiplier_ * seed_ + increment_, modulus_);
    if (seed_<0.0) seed_+=modulus_;
    return seed_;
}

