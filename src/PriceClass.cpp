#include <cmath>
#include <iostream>
#include <random>
#include "PriceClass.hpp"
#include "LookbackOption.hpp"


#include "AssetHistory.h"
#include "Util.h"

double PricingEngine::calculatePriceNaive(const Option &option, double S, double r, double sigma, unsigned int nSimulations) {
    
    // Convert pointer to base option class to LookbackOption
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    
    if (!lookbackOption) {
        // If option is not a LookbackOption, return 0.0
        return 0.0;
    }
    
    // Using normal distribution from AssetHistory to generate random numbers
    std::normal_distribution<double> dist(0.0, 1.0);
    
    // Divide the option expiry into discrete monitoring periods
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriod();
    double sumPayoffs = 0.0;
    
    // Loop over simulation paths.
    for (unsigned int i = 0; i < nSimulations; ++i) {
        double spotPath = S;
        double extreme = S; // Initialise extreme value with the starting spot.
        
        // For each monitoring period, simulate the asset price evolution.
        // (For j==0 we already have the initial price.)
        for (unsigned int j = 0; j < lookbackOption->getMonitoringPeriods(); ++j) {
            if (j > 0) {
                double randNormal = dist(AssetHistory::RANDOM_GENERATOR);
                spotPath *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt +
                                     volatility * std::sqrt(dt) * randNormal);
            }
            
            // Update the extreme value:
            // For a call, we track the maximum price.
            // For a put, we track the minimum price.
            if (lookbackOption->getOptionType() == Option::Call) {
                extreme = std::max(extreme, spotPath);
            } else { // put
                extreme = std::min(extreme, spotPath);
            }
        }
        
        // Evaluate the payoff based on the extreme value.
        sumPayoffs += option.payoff(extreme);
    }
    
    // Discount the average payoff back to present value.
    double price = (sumPayoffs / numSimulations) * std::exp(-riskFreeRate * lookbackOption->getExpiry());
    return price;
}

double PriceClass::calculateP_Antithetic(const Option &option, double S, double r, double sigma, double T, unsigned int nSimulations) {
    // Convert pointer to base option class to LookbackOption
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        // If option is not a LookbackOption, return 0.0
        return 0.0;
    }

    // Using normal distribution from AssetHistory to generate random numbers
    std::normal_distribution<double> dist(0.0, 1.0);

    // Time increment based on number of periods
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriod();
    double sumPayoffs = 0.0;

    // Simulate nSimulations paths with antithetic variates
    for (unsigned int i = 0; i < nSimulations; ++i){
        // Initialise normal and antithetic random variables
        double spotPath = S;
        double extreme = S;
        double antitheticSpotPath = S;
        double antitheticExtreme = S;

        // Loop over periods
        for (unsigned int j = 0; hj < lookbackOption->getPeriod(); ++j) {
            if (j > 0) {
                // Generate random numbers
                double randNormal = dist(AssetHistory::RANDOM_GENERATOR);
                double randAntithetic = -randNormal;

                // Simulate spot price evolution
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randNormal);
                antitheticSpotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randAntithetic);
            }

            // Update extreme values
            if (lookbackOption->getOptionType() == Option::Call) {
                extreme = std::max(extreme, spotPath);
                antitheticExtreme = std::max(antitheticExtreme, antitheticSpotPath);
            } else {
                extreme = std::min(extreme, spotPath);
                antitheticExtreme = std::min(antitheticExtreme, antitheticSpotPath);
            }
        }§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§


    }