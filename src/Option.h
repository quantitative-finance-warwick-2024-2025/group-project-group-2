#ifndef OPTION_H // prevents header file from being processed multiple times
#define OPTION_H // if not defined, define it

class Option {
public:
    enum Type { Call, Put }; // create new type

    // constructor
    Option(double K, double T, Type type); //member variables to store type, K, T

    virtual double payoff(double S) const = 0; // payoff given underlying, pure virtual function

    // accessors

    double getStrike() const; // get K
    double getExpiry() const; // get T
    Type getType() const; // get type

// We want to allow inheritance while preventing external access
protected:
    double K; // strike price
    double T; // expiry
    Type type; // type of option
};

#endif // OPTION_H