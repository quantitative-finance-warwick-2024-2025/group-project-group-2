#include <iostream>
#include <cmath>
#include "LookbackOption.h"
#include "PriceClass.h"

int main() {
    // Common parameters
    double S      = 75.0;
    double K      = 80.0;
    double r      = 0.02;
    double sigma  = 0.1;
    double T      = 3.0;

    // Test a lookback call, fixed strike
    unsigned int periods = 10;
    LookbackOption lookbackCall(K, T, Option::Call, periods, LookbackOption::Fixed);
    LookbackOption lookbackPut(K, T, Option::Put, periods, LookbackOption::Fixed);


    // 1) Convergence over number of simulations:
    std::cout << "Convergence over Number of Simulations\n";
    std::cout << "nSim, Call Naive, Call Antithetic, Call Strat_Sampling, Call Control Variates, Put Naive, Put Antithetic, Put Strat_Sampling, Put Control Variates\n";
    for (unsigned int nSim : {10, 20, 30, 40, 50, 100, 200, 300, 500, 1000, 5000, 10000}) {
        double naivePriceC = PriceClass::calculateP_Naive(lookbackCall, S, r, sigma, T, nSim);
        double antiPriceC  = PriceClass::calculateP_Antithetic(lookbackCall, S, r, sigma, T, nSim);
        double StratSamPriceC  = PriceClass::calculateP_StratifiedSampling(lookbackCall, S, r, sigma, T, nSim, 10);
        double ConVarPriceC  = PriceClass::calculateP_ControlVariates(lookbackCall, S, r, sigma, T, nSim);
        
        double naivePriceP = PriceClass::calculateP_Naive(lookbackPut, S, r, sigma, T, nSim);
        double antiPriceP  = PriceClass::calculateP_Antithetic(lookbackPut, S, r, sigma, T, nSim);
        double StratSamPriceP  = PriceClass::calculateP_StratifiedSampling(lookbackPut, S, r, sigma, T, nSim, 10);
        double ConVarPriceP  = PriceClass::calculateP_ControlVariates(lookbackPut, S, r, sigma, T, nSim);

        std::cout << nSim << "," << naivePriceC << "," << antiPriceC << "," << StratSamPriceC << "," << ConVarPriceC << ","<< naivePriceP << "," << antiPriceP << "," << StratSamPriceP << ","  << ConVarPriceP << "\n";    
    }

    // 2) Convergence over number of periods:
    std::cout << "\nConvergence over periods\n";
    std::cout << "periods, Call Naive, Call Antithetic, Call Strat_Sampling, Call Control Variates, Put Naive, Put Antithetic, Put Strat_Sampling, Put Control Variates\n";
    unsigned int nSimConvergence = 1000;
    for (unsigned int p : {1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 1<<9, 1<<10, 1<<11}) {
        LookbackOption lbCall(K, T, Option::Call, p, LookbackOption::Fixed);
        LookbackOption lbPut(K, T, Option::Put, p, LookbackOption::Fixed);
    
        double naivePriceC = PriceClass::calculateP_Naive(lbCall, S, r, sigma, T, nSimConvergence);
        double antiPriceC  = PriceClass::calculateP_Antithetic(lbCall, S, r, sigma, T, nSimConvergence);
        double StratSamPriceC  = PriceClass::calculateP_StratifiedSampling(lbCall, S, r, sigma, T, nSimConvergence, 10);
        double ConVarPriceC  = PriceClass::calculateP_ControlVariates(lbCall, S, r, sigma, T, nSimConvergence);

        double naivePriceP = PriceClass::calculateP_Naive(lbPut, S, r, sigma, T, nSimConvergence);
        double antiPriceP  = PriceClass::calculateP_Antithetic(lbPut, S, r, sigma, T, nSimConvergence);
        double StratSamPriceP  = PriceClass::calculateP_StratifiedSampling(lbPut, S, r, sigma, T, nSimConvergence, 10);
        double ConVarPriceP  = PriceClass::calculateP_ControlVariates(lbPut, S, r, sigma, T, nSimConvergence);
    
        std::cout << p << "," << naivePriceC << "," << antiPriceC << "," << StratSamPriceC << "," << ConVarPriceC << ","<< naivePriceP << "," << antiPriceP << "," << StratSamPriceP << ","  << ConVarPriceP << "\n";
    }

    return 0;
}
