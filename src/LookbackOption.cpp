#include <algorithm>
#include "LookbackOption.h"

//Constructor for LookbackOption
//Initialises the LookbackOption with the given parameters
// Also stores the number of periods that are monitored

LookbackOption::LookbackOption(double K, double T, Option::Type optionType, unsigned int periods, StrikeType strikeType) 
    : Option(K,T, optionType), strikeType(strikeType), periods(periods) {}

// Payoff function that computes the option payoff based on the final asset price & extreme values
// For a fixed strike:
//      Call - payoff = max(extreme - strike, 0)
//      Put - payoff = max(strike - extreme, 0)
// For a floating strike:
//      Call - payoff = max(S_T - extreme, 0)
//      Put - payoff = max(extreme - S_T, 0)

double LookbackOption::payoff(double S_T, double extreme) const {
    if (getStrikeType() == StrikeType::Fixed) {
        if (getType() == Option::Call) {
            return std::max(extreme - K, 0.0);
        } else { // Put
            return std::max(K - extreme, 0.0);
        }
    } else { // StrikeType::Floating
        if (getType() == Option::Call) {
            return std::max(S_T - extreme, 0.0);
        } else { // Put 
            return std::max(extreme - S_T, 0.0);
        }
    }
}

unsigned int LookbackOption::getPeriods() const {
    return periods;
}

LookbackOption::StrikeType LookbackOption::getStrikeType() const {
    return strikeType;
}