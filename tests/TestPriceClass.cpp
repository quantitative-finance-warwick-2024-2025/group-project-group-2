#define CATCH_CONFIG_MAIN
#include "../extern/catch2/catch.hpp"
#include "../src/PriceClass.h"
#include "../src/LookbackOption.h"


TEST_CASE("PriceClass: Monte Carlo Pricing (Naive) - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 10000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Antithetic) - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 10000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);

    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 10000);
    double antitheticPrice = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 10000);

    REQUIRE(naivePrice > 0.0);
    REQUIRE(antitheticPrice > 0.0);

    double tolerance = 0.05 * naivePrice;
    REQUIRE(std::fabs(naivePrice - antitheticPrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Naive) - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 10000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Antithetic) - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 10000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);

    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 10000);
    double antitheticPrice = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 10000);

    REQUIRE(naivePrice > 0.0);
    REQUIRE(antitheticPrice > 0.0);

    double tolerance = 0.05 * naivePrice;
    REQUIRE(std::fabs(naivePrice - antitheticPrice) < tolerance);
}

