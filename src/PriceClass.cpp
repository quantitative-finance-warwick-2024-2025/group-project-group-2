#include <cmath>
#include <iostream>
#include <random>
#include <numeric>

#include "PriceClass.h"
#include "LookbackOption.h"
#include "AssetHistory.h"
#include "Util.h"

double PriceClass::calculateP_Naive(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations) {
    
    // Convert pointer to base option class to LookbackOption
    // Want to check that the given option object is a LookbackOption object
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    
    
    // Using normal distribution from AssetHistory to generate random numbers
    std::normal_distribution<double> dist(0.0, 1.0);
    
    // Divide the option expiry into discrete time periods
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriods();
    double sumPayoffs = 0.0;
    
    // Loop over simulation paths.
    for (unsigned int i = 0; i < nSimulations; ++i) {
        double spotPath = S;
        double extreme = S; // Initialise extreme value with the starting spot.
        
        // For each monitoring period, simulate the asset price evolution.
        // (For j==0 we already have the initial price.)
        for (unsigned int j = 0; j < lookbackOption->getPeriods(); ++j) {
            if (j > 0) {
                double randNormal = dist(AssetHistory::get_random_generator());
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt +
                                     sigma * std::sqrt(dt) * randNormal);
            }
            
            // Update the extreme value:
            // For a Fixed strike call, we track the maximum price.
            // For a Fixed strike put, we track the minimum price.
            // For a Floating Strike call, we track the minimum price
            // For a Floating Strike Put, we track the maximum price
            if ((lookbackOption->getType() == Option::Call) & (lookbackOption->getStrikeType() == LookbackOption::Fixed)) {
                extreme = std::max(extreme, spotPath);
            } else if ((lookbackOption->getType() == Option::Put) & (lookbackOption->getStrikeType() == LookbackOption::Fixed)) { // put
                extreme = std::min(extreme, spotPath);
            } else if ((lookbackOption->getType() == Option::Call) & (lookbackOption->getStrikeType() == LookbackOption::Floating)){
                extreme = std::min(extreme, spotPath);
            } else if ((lookbackOption->getType() == Option::Put) & (lookbackOption->getStrikeType() == LookbackOption::Floating)){
                extreme = std::max(extreme, spotPath);
            }
        }
        
        // Evaluate the payoff based on the extreme value.
        sumPayoffs += option.payoff(extreme);
    }
    
    // Discount the average payoff back to present value.
    double price = (sumPayoffs / nSimulations) * std::exp(-r * lookbackOption->getExpiry());
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
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriods();
    double sumPayoffs = 0.0;

    // Simulate nSimulations paths with antithetic variates
    for (unsigned int i = 0; i < nSimulations; ++i){
        // Initialise normal and antithetic random variables
        double spotPath = S;
        double extreme = S;
        double antitheticSpotPath = S;
        double antitheticExtreme = S;

        // Loop over periods
        for (unsigned int j = 0; j < lookbackOption->getPeriods(); ++j) {
            if (j > 0) {
                // Generate random numbers
                double randNormal = dist(AssetHistory::get_random_generator());
                double randAntithetic = -randNormal;

                // Simulate spot price evolution
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randNormal);
                antitheticSpotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randAntithetic);
            }

            // Update extreme values
            if (lookbackOption->getType() == Option::Call) {
                extreme = std::max(extreme, spotPath);
                antitheticExtreme = std::max(antitheticExtreme, antitheticSpotPath);
            } else {
                extreme = std::min(extreme, spotPath);
                antitheticExtreme = std::min(antitheticExtreme, antitheticSpotPath);
            }
        }

        // Evaluate the payoff for both paths.
        double payoff1 = lookbackOption->payoff(spotPath, extreme);
        double payoff2 = lookbackOption->payoff(antitheticSpotPath, antitheticExtreme);
        double avgPayoff = (payoff1 + payoff2) / 2.0;
        sumPayoffs += avgPayoff;

    }

    // Discount the average payoff back to present value.
    double price = (sumPayoffs / nSimulations) * std::exp(-r * lookbackOption->getExpiry());
    return price;
}


double normal_cdf(double x) {
    return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));
}
double normal_ppf(double p, double tol = 1e-6) {
    // Calculate the quantile of standard normal distribution
    if (p <= 0 || p >= 1) {
        throw std::invalid_argument("p must be in (0,1)");
    }
    double low = -10, high = 10, mid;
    while (high - low > tol) {
        mid = (low + high) / 2;
        if (normal_cdf(mid) < p)
            low = mid;
        else
            high = mid;
    }
    return mid;
}
void brownian_bridge(std::vector<double>&W, std::vector<double>& times, int left, int right, double t0, double t1) {
    if (right - left <= 1) return; 

    std::random_device rd;
    std::mt19937 gen(rd());
    int mid = (left + right) / 2;
    double t_mid = (t0 + t1) / 2;

    double std_variance = std::sqrt((t1 - t0) / 4.0);
    std::normal_distribution<double> dist(0.0, 1.0);

    W[mid] = (W[left] + W[right]) / 2.0 + std_variance * dist(gen);
    times[mid] = t_mid;
    brownian_bridge(W, times, left, mid, t0, t_mid);
    brownian_bridge(W, times, mid, right, t_mid, t1);
}
double get_extreme(double S, double r, double sigma, double ST, double T, int log_periods,Option::Type optionType){
  int N = 1 << log_periods;
  std::vector<double> W(N + 1);
  std::vector<double> times(N + 1);
  std::vector<double> St(N + 1);
  W[0] = 0;
  W[N] = (std::log(ST / S) - (r - 0.5 * sigma * sigma) * T)/(sigma * std::sqrt(T));
  times[0] = 0.0;
  times[N] = T;
  brownian_bridge(W, times, 0, N, 0.0, T);
  for (int i = 0; i <= N; ++i) {
    St[i] = S * std::exp((r - 0.5 * sigma * sigma) * times[i] + sigma * W[i]);
  }

  double extreme;
  if (optionType == Option::Call) {
    extreme = *std::max_element(St.begin(), St.end());
  } else { // put
    extreme = *std::min_element(St.begin(), St.end());
  }

  return extreme;
}
double PriceClass::calculateP_StratifiedSampling(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations, unsigned int mStrata){
    // Convert pointer to base option class to LookbackOption
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        // If option is not a LookbackOption, return 0.0
        return 0.0;
    }

    double price = 0.0;
    static std::random_device rd; 
    static std::mt19937 gen(rd());
    for (unsigned int i = 0; i < mStrata; ++i){
        double iPayoff = 0.0; 
        
        // Generate a uniform random variable within the i-th stratum range [start, end)
        double start = static_cast<double> (i) / mStrata;
        double end = static_cast<double>(i+1)/ mStrata;
        std::uniform_real_distribution<double> dist(start, end);
        
        // nSimulations/mStrata:the number of simulations in each layer
        for (unsigned int j = 0; j < nSimulations/mStrata; ++j){             
            double randUniform = dist(gen);
            // Simulate j different values of W_N in the layer i
            double randomterm = sigma*std::sqrt(lookbackOption->getExpiry())*normal_ppf(randUniform);
            // Simulate S_T from each W_N
            double ST = S*std::exp((r - 0.5 * sigma * sigma) * lookbackOption->getExpiry() + randomterm);
            double extreme = get_extreme(S, r, sigma, ST, T, 13, lookbackOption->getType());
            double jPayoff = option.payoff(extreme);

            // Compute the payoff in layer i
            iPayoff += jPayoff / (static_cast<double>(nSimulations)/mStrata);
        }
        // Compute the equally weighted layer-payoff 
        price += iPayoff/mStrata;
    } 
    price *= std::exp(-r * lookbackOption->getExpiry());     
    return price;
}

double covariance(const std::vector<double>& X, const std::vector<double>& Y) {
    if (X.size() != Y.size() || X.empty()) {
        throw std::invalid_argument("Vectors must have the same non-zero length.");
    }
  
    double meanX = std::accumulate(X.begin(), X.end(), 0.0) / X.size();
    double meanY = std::accumulate(Y.begin(), Y.end(), 0.0) / Y.size();
    double cov = 0.0;
    int n = X.size();
  
    for (int i = 0; i < n; ++i) {
        cov += (X[i] - meanX) * (Y[i] - meanY);
    }
  
    return cov / n;
}
double PriceClass::calculateP_ControlVariates(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations){
    // Convert pointer to base option class to LookbackOption
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        // If option is not a LookbackOption, return 0.0
        return 0.0;
    }

    double price =0.0;
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriods();
    std::vector<double> payoffs(nSimulations);
    std::vector<double> ST(nSimulations);
    std::vector<double> adjustedpayoffs(nSimulations);
    std::normal_distribution<double> dist(0.0, 1.0);

    for (unsigned int i = 0; i < nSimulations; ++i){
        double spotPath = S;
        double extreme = S; 
        for (unsigned int j = 0; j < lookbackOption->getPeriods(); ++j) {
            if (j > 0) {
                double randNormal = dist(AssetHistory::get_random_generator());
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt +
                                     sigma * std::sqrt(dt) * randNormal);
            }
            
            if (lookbackOption->getType() == Option::Call) {
                extreme = std::max(extreme, spotPath);
            } else { // put
                extreme = std::min(extreme, spotPath);
            }
        }
        ST[i] = spotPath;
        payoffs[i]= option.payoff(extreme);
    }   
    double cov=covariance(payoffs,ST);
    double var=covariance(ST,ST);
    double c=cov/var;
    double ST_bar=std::accumulate(ST.begin(), ST.end(), 0.0) / ST.size();
    for (unsigned int i = 0; i < nSimulations; ++i){
        adjustedpayoffs[i]=payoffs[i]-c*(ST[i]-ST_bar);         
    } 

    price = std::exp(-r * lookbackOption->getExpiry()) * std::accumulate(adjustedpayoffs.begin(), adjustedpayoffs.end(), 0.0) / adjustedpayoffs.size();
    return price;
}