#include "Option.h"

// constructor, intialise member variables
Option::Option(double K, double T, Type type) 
    : K(K), T(T), type(type) {}

double Option::getStrike() const {
    return K;
}

double Option::getExpiry() const {
    return T;
}

Option::Type Option::getType() const {
    return type;
}