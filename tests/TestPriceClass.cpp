#define CATCH_CONFIG_MAIN
#include "../extern/catch2/catch.hpp"
#include "../src/PriceClass.h"
#include "../src/LookbackOption.h"


TEST_CASE("PriceClass: Monte Carlo Pricing (Naive) - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Antithetic) - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);

    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double antitheticPrice = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);

    REQUIRE(naivePrice > 0.0);
    REQUIRE(antitheticPrice > 0.0);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(naivePrice - antitheticPrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Naive) - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Antithetic) - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);

    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double antitheticPrice = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);

    REQUIRE(naivePrice > 0.0);
    REQUIRE(antitheticPrice > 0.0);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(naivePrice - antitheticPrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Stratified Sampling) - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_StratifiedSampling(option, 100, 0.05, 0.2, 1.0, 100, 5);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Stratified Sampling) - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_StratifiedSampling(option, 100, 0.05, 0.2, 1.0, 100, 5);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Control Variates) - Call, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Control Variates) - Put, Fixed", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double price = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Control Variates vs Naive (Call, Fixed)", "[pricing][accuracy]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double controlPrice = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(controlPrice - naivePrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Control Variates vs Naive (Put, Fixed)", "[pricing][accuracy]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double controlPrice = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(controlPrice - naivePrice) < tolerance);
}



// Floating 

TEST_CASE("PriceClass: Monte Carlo Pricing (Naive) - Call, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Antithetic) - Call, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Call, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);

    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double antitheticPrice = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);

    REQUIRE(naivePrice > 0.0);
    REQUIRE(antitheticPrice > 0.0);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(naivePrice - antitheticPrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Naive) - Put, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Antithetic) - Put, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Put, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);

    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double antitheticPrice = PriceClass::calculateP_Antithetic(option, 100, 0.05, 0.2, 1.0, 1000);

    REQUIRE(naivePrice > 0.0);
    REQUIRE(antitheticPrice > 0.0);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(naivePrice - antitheticPrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Stratified Sampling) - Call, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_StratifiedSampling(option, 100, 0.05, 0.2, 1.0, 100, 5);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Stratified Sampling) - Put, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_StratifiedSampling(option, 100, 0.05, 0.2, 1.0, 100, 5);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Control Variates) - Call, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing (Control Variates) - Put, Floating", "[pricing]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);
    double price = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);
    REQUIRE(price > 0.0);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Control Variates vs Naive (Call, Floating)", "[pricing][accuracy]") {
    LookbackOption option(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);
    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double controlPrice = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(controlPrice - naivePrice) < tolerance);
}

TEST_CASE("PriceClass: Monte Carlo Pricing Accuracy - Control Variates vs Naive (Put, Floating)", "[pricing][accuracy]") {
    LookbackOption option(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);
    double naivePrice = PriceClass::calculateP_Naive(option, 100, 0.05, 0.2, 1.0, 1000);
    double controlPrice = PriceClass::calculateP_ControlVariates(option, 100, 0.05, 0.2, 1.0, 1000);

    double tolerance = 0.15 * naivePrice;
    REQUIRE(std::fabs(controlPrice - naivePrice) < tolerance);
}