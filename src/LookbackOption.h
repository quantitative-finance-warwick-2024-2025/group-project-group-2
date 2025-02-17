#ifndef LOOKBACKOPTION_H
#define LOOKBACKOPTION_H

#include "Option.h"
#include <algorithm>

class LookbackOption : public Option {
public:
    // Define the strike type of lookback option
    enum StrikeType {
        Fixed,
        Floating
    };

    // constructor
    // For fixed strikes, 'strike' is used in the payoff calculation
    // For floating strikes, the final price determines the effective strike
    LookbackOption(double K, double T, Option::Type optionTypem, unsigned int periods, StrikeType strikeType);

    // Payoff function calculates the option payoff given final asset price and extreme
    // For a fixed strike:
    //      Call - payoff = max(extreme - strike, 0)
    //      Put - payoff = max(strike - extreme, 0)
    // For a floating strike:
    //      Call - payoff = max(S_T - extreme, 0)
    //      Put - payoff = max(extreme - S_T, 0)

    double payoff(double S_T, double extreme) const;

    // Returns the number of periods used to track underlying
    unsigned int getPeriods() const;

    // Returns the strike type of the option
    StrikeType getStrikeType() const;

// Only memeber functions can access these values
private:
    unsigned int periods; // Number of periods monitored
    StrikeType strikeType; // Whether fixed or floating

};

#endif // LOOKBACKOPTION_H