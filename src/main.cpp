#include <iostream>
#include "Option.h"
#include "LookbackOption.h"
#include "PriceClass.h"

int main()
{
	double S = 75.0; // spot price
	double K = 80.0; // strike price
	double r = 0.02; // risk-free rate
	double sigma = 0.1; // volatility
	double T = 1.0; // expiry
	unsigned int nSimulations = 1000; // number of simulations
	unsigned int periods = 1000;


	// Create a lookback call option
	LookbackOption lookbackCall(K, T, Option::Call, periods, LookbackOption::Floating);
	LookbackOption lookbackPut(K, T, Option::Put, periods, LookbackOption::Floating);

	double callP_Naive = PriceClass::calculateP_Naive(lookbackCall, S, r, sigma, T, nSimulations);
	double putP_Naive = PriceClass::calculateP_Naive(lookbackPut, S, r, sigma, T, nSimulations);
	double callP_Antithetic = PriceClass::calculateP_Antithetic(lookbackCall, S, r, sigma, T, nSimulations);
	double putP_Antithetic = PriceClass::calculateP_Antithetic(lookbackPut, S, r, sigma, T, nSimulations);
	
	std::cout << "Lookback Call Price (Naive): " << callP_Naive << std::endl;
	std::cout << "Lookback Put Price (Naive): " << putP_Naive << std::endl;
	std::cout << "Lookback Call Price (Antithetic): " << callP_Antithetic << std::endl;
	std::cout << "Lookback Put Price (Antithetic): " << putP_Antithetic << std::endl;


	return 0;
}