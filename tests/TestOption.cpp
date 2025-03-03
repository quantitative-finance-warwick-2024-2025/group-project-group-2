#define CATCH_CONFIG_MAIN
#include "../extern/catch2/catch.hpp"
#include "../src/Option.h"

class TestOption : public Option {
public:
    TestOption(double K, double T, Type type) : Option(K, T, type) {}
    double payoff(double S) const override { return (S - K > 0) ? S - K : 0; }
};

TEST_CASE("Option: Constructor and Get", "[option]") {
    TestOption callOption(100.0, 1.0, Option::Call);
    TestOption putOption(100.0, 1.0, Option::Put);

    REQUIRE(callOption.getStrike() == Approx(100.0));
    REQUIRE(callOption.getExpiry() == Approx(1.0));
    REQUIRE(callOption.getType() == Option::Call);

    REQUIRE(putOption.getStrike() == Approx(100.0));
    REQUIRE(putOption.getExpiry() == Approx(1.0));
    REQUIRE(putOption.getType() == Option::Put);
}

TEST_CASE("Option: Payoff Calculation", "[option]") {
    TestOption callOption(100.0, 1.0, Option::Call);
    
    REQUIRE(callOption.payoff(120.0) == Approx(20.0));
    REQUIRE(callOption.payoff(90.0) == Approx(0.0));
}
