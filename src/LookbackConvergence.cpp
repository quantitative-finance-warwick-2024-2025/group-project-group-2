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
    double T      = 1.0;

    // Test a lookback call, fixed strike
    unsigned int periods = 10;
    LookbackOption lookbackCall(K, T, Option::Call, periods, LookbackOption::Fixed);
    LookbackOption lookbackPut(K, T, Option::Put, periods, LookbackOption::Fixed);


    // 1) Convergence over number of simulations:
    std::cout << "Convergence over nSim\n";
    std::cout << "nSim, Call Naive, Call Antithetic, Put Naive, Put Antithetic\n";
    for (unsigned int nSim : {1000, 5000, 10000, 50000, 100000, 500000, 1000000}) {
        double naivePriceC = PriceClass::calculateP_Naive(lookbackCall, S, r, sigma, T, nSim);
        double antiPriceC  = PriceClass::calculateP_Antithetic(lookbackCall, S, r, sigma, T, nSim);
        double naivePriceP = PriceClass::calculateP_Naive(lookbackPut, S, r, sigma, T, nSim);
        double antiPriceP  = PriceClass::calculateP_Antithetic(lookbackPut, S, r, sigma, T, nSim);
        std::cout << nSim << "," << naivePriceC << "," << antiPriceC << "," << naivePriceP << "," << antiPriceP << "\n";
    }

    // 2) Convergence over number of periods:
    std::cout << "\nConvergence over periods\n";
    std::cout << "periods,Call Naive, Call Antithetic, Put Naive, Put Antithetic\n";
    unsigned int nSimConvergence = 100000; // fix a large sim count
    for (unsigned int p : {5, 10, 20, 50, 100, 200, 500}) {
        LookbackOption lbCall(K, T, Option::Call, p, LookbackOption::Fixed);
        LookbackOption lbPut(K, T, Option::Put, p, LookbackOption::Fixed);
        double naivePriceC = PriceClass::calculateP_Naive(lbCall, S, r, sigma, T, nSimConvergence);
        double antiPriceC  = PriceClass::calculateP_Antithetic(lbCall, S, r, sigma, T, nSimConvergence);
        double naivePriceP = PriceClass::calculateP_Naive(lbPut, S, r, sigma, T, nSimConvergence);
        double antiPriceP  = PriceClass::calculateP_Antithetic(lbPut, S, r, sigma, T, nSimConvergence);
        std::cout << p << "," << naivePriceC << "," << antiPriceC << "," << naivePriceP << "," << antiPriceP << "\n";
    }

    return 0;
}
