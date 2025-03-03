#define CATCH_CONFIG_MAIN
#include "../extern/catch2/catch.hpp"
#include "../src/LookbackOption.h"

TEST_CASE("LookbackOption: Payoff", "[lookback]") {
    LookbackOption fixedCall(100.0, 1.0, Option::Call, 10, LookbackOption::Fixed);
    LookbackOption fixedPut(100.0, 1.0, Option::Put, 10, LookbackOption::Fixed);
    LookbackOption floatingCall(100.0, 1.0, Option::Call, 10, LookbackOption::Floating);
    LookbackOption floatingPut(100.0, 1.0, Option::Put, 10, LookbackOption::Floating);

    SECTION("Fixed Strike Call Option Payoff") {
        REQUIRE(fixedCall.payoff(120, 130) == Approx(30.0));
        REQUIRE(fixedCall.payoff(120, 90) == Approx(0.0));
    }

    SECTION("Fixed Strike Put Option Payoff") {
        REQUIRE(fixedPut.payoff(120, 90) == Approx(10.0));
        REQUIRE(fixedPut.payoff(120, 130) == Approx(0.0));
    }

    SECTION("Floating Strike Call Option Payoff") {
        REQUIRE(floatingCall.payoff(120, 90) == Approx(30.0));
        REQUIRE(floatingCall.payoff(120, 130) == Approx(0.0));
    }

    SECTION("Floating Strike Put Option Payoff") {
        REQUIRE(floatingPut.payoff(120, 130) == Approx(10.0));
        REQUIRE(floatingPut.payoff(120, 90) == Approx(0.0));
    }
}

TEST_CASE("LookbackOption: Get", "[lookback]") {
    LookbackOption option(100.0, 1.0, Option::Call, 12, LookbackOption::Floating);

    REQUIRE(option.getStrikeType() == LookbackOption::Floating);
    REQUIRE(option.getPeriods() == 12);
}
