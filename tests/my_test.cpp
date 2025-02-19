#include "../src/PriceClass.h"
#include "../src/LookbackOption.h"

class PriceClassTest : public ::testing::Test {
protected:
	// Initialise objects needed for testing

	void Initialise() override {
		prcingClass = new PriceClass();
		double K = 80.0; // strike price
		double T = 1.0; // expiry
		double T_edge = 0.01; // edge case expiry
		unsigned int periods = 10; // number of periods


		// Initialise lookback options with fixed strikes
		LookbackOption::StrikeType strikeTypeFixed = LookbackOption::StrikeType::Fixed;
		callOption = new LookbackOption(K, T, Option::Type::Call, periods, strikeTypeFixed);
		putOption = new LookbackOption(K, T, Option::Type::Put, periods, strikeTypeFixed);
		callOption_edge = new LookbackOption(K, T_edge, Option::Type::Call, periods, strikeTypeFixed);

		// Initialise lookback options with floating strikes
		LookbackOption::StrikeType strikeTypeFloating = LookbackOption::StrikeType::Floating;
		callOptionFloating = new LookbackOption(K, T, Option::Type::Call, periods, strikeTypeFloating);
		putOptionFloating = new LookbackOption(K, T, Option::Type::Put, periods, strikeTypeFloating);
		callOptionFloating_edge = new LookbackOption(K, T_edge, Option::Type::Call, periods, strikeTypeFloating);

		// Initialise other parameters
		S = 75.0; // spot price
		r = 0.02; // risk-free rate
		sigma = 0.1; // volatility
		nSimulations = 250000; // number of simulations
		S_edge = 0.1; // edge case spot price
	}

	// Free resources allocated for testing
	void TearDown() override {
		delete prcingClass;
		delete callOption;
		delete putOption;
		delete callOptionFloating;
		delete putOptionFloating;
		delete callOption_edge;
	}

	PriceClass* prcingClass;
	LookbackOption* callOption;
	LookbackOption* putOption;
	LookbackOption* callOption_edge;
	
	LookbackOption* callOptionFloating;
	LookbackOption* putOptionFloating;
	LookbackOption* callOptionFloating_edge
	
	double S{};
	double S_edge{};
	double r{};
	double sigma{};
	unsigned int nSimulations{};

};

// Test case for ensuring all prices are greater than 0 for both strike types
TEST_F(PriceClassTest, NaiveCallGreaterZero){
	double priceFixed PriceClass::calculateP_Naive(*callOption, S, r, sigma, T, nSimulations);
	double priceFloating PriceClass::calculateP_Naive(*callOptionFloating, S, r, sigma, T, nSimulations);
	EXPECT_GT(priceFixed, 0);
	EXPECT_GT(priceFloating, 0);
}

TEST_F(PriceClassTest, NaivePutGreaterZero){
	double priceFixed PriceClass::calculateP_Naive(*putOption, S, r, sigma, T, nSimulations);
	double priceFloating PriceClass::calculateP_Naive(*putOptionFloating, S, r, sigma, T, nSimulations);
	EXPECT_GT(priceFixed, 0);
	EXPECT_GT(priceFloating, 0);
}



TEST_F(PriceClassTest, AntitheticCallGreaterZero){
	double priceFixed PriceClass::calculateP_Antithetic(*callOption, S, r, sigma, T, nSimulations);
	double priceFloating PriceClass::calculateP_Antithetic(*callOptionFloating, S, r, sigma, T, nSimulations);
	EXPECT_GT(priceFixed, 0);
	EXPECT_GT(priceFloating, 0);
}

TEST_F(PriceClassTest, NaivePutGreaterZero){
	double priceFixed PriceClass::calculateP_Naive(*putOption, S, r, sigma, T, nSimulations);
	double priceFloating PriceClass::calculateP_Naive(*putOptionFloating, S, r, sigma, T, nSimulations);
	EXPECT_GT(priceFixed, 0);
	EXPECT_GT(priceFloating, 0);
}