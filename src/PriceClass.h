#pragma once // include guard

#include "Option.h"

class PriceClass {
public:
    static double calculateP_Naive(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations);
    static double calculateP_Antithetic(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations);
    static double calculateP_StratifiedSampling(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations, unsigned int mStrata);
    static double calculateP_ControlVariates(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations); 

};