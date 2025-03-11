#include <cmath>
#include <iostream>
#include <random>
#include <numeric>

#include "PriceClass.h"
#include "LookbackOption.h"
#include "AssetHistory.h"
#include "Util.h"

// Helper function to update the extreme value based on option type and strike type.
static double updateExtreme(double currentExtreme, double spotPath, const LookbackOption* lookbackOption) {
    if ((lookbackOption->getType() == Option::Call) && (lookbackOption->getStrikeType() == LookbackOption::Fixed)) {
        // For a fixed-strike call, track the maximum price.
        return std::max(currentExtreme, spotPath);
    } else if ((lookbackOption->getType() == Option::Put) && (lookbackOption->getStrikeType() == LookbackOption::Fixed)) {
        // For a fixed-strike put, track the minimum price.
        return std::min(currentExtreme, spotPath);
    } else if ((lookbackOption->getType() == Option::Call) && (lookbackOption->getStrikeType() == LookbackOption::Floating)) {
        // For a floating-strike call, track the minimum price.
        return std::min(currentExtreme, spotPath);
    } else if ((lookbackOption->getType() == Option::Put) && (lookbackOption->getStrikeType() == LookbackOption::Floating)) {
        // For a floating-strike put, track the maximum price.
        return std::max(currentExtreme, spotPath);
    }
    return currentExtreme; // Default fallback (should never be reached if types are set correctly).
}

double PriceClass::calculateP_Naive(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations) {
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        return 0.0;
    }
    
    std::normal_distribution<double> dist(0.0, 1.0);
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriods();
    double sumPayoffs = 0.0;
    
    for (unsigned int i = 0; i < nSimulations; ++i) {
        double spotPath = S;
        double extreme = S; // Initialize extreme with the starting price.
        
        for (unsigned int j = 0; j < lookbackOption->getPeriods(); ++j) {
            if (j > 0) {
                double randNormal = dist(AssetHistory::get_random_generator());
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randNormal);
            }
            extreme = updateExtreme(extreme, spotPath, lookbackOption);
        }
        
        // Use the appropriate payoff function based on strike type.
        if (lookbackOption->getStrikeType() == LookbackOption::Floating) {
            sumPayoffs += lookbackOption->payoff(spotPath, extreme);
        } else {
            sumPayoffs += option.payoff(extreme);
        }
    }
    
    double price = (sumPayoffs / nSimulations) * std::exp(-r * lookbackOption->getExpiry());
    return price;
}


double PriceClass::calculateP_Antithetic(const Option &option, double S, double r, double sigma, double T, unsigned int nSimulations) {
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        return 0.0;
    }

    std::normal_distribution<double> dist(0.0, 1.0);
    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriods();
    double sumPayoffs = 0.0;

    for (unsigned int i = 0; i < nSimulations; ++i) {
        double spotPath = S;
        double antitheticSpotPath = S;
        double extreme = S;
        double antitheticExtreme = S;

        for (unsigned int j = 0; j < lookbackOption->getPeriods(); ++j) {
            if (j > 0) {
                double randNormal = dist(AssetHistory::get_random_generator());
                double randAntithetic = -randNormal;
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randNormal);
                antitheticSpotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randAntithetic);
            }
            extreme = updateExtreme(extreme, spotPath, lookbackOption);
            antitheticExtreme = updateExtreme(antitheticExtreme, antitheticSpotPath, lookbackOption);
        }
        
        double payoff1, payoff2;
        if (lookbackOption->getStrikeType() == LookbackOption::Floating) {
            payoff1 = lookbackOption->payoff(spotPath, extreme);
            payoff2 = lookbackOption->payoff(antitheticSpotPath, antitheticExtreme);
        } else {
            payoff1 = option.payoff(extreme);
            payoff2 = option.payoff(antitheticExtreme);
        }
        double avgPayoff = (payoff1 + payoff2) / 2.0;
        sumPayoffs += avgPayoff;
    }

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
double get_extreme(double S, double r, double sigma, double ST, double T, int log_periods,Option::Type optionType, LookbackOption::StrikeType strikeType){
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
  if (strikeType == LookbackOption::Floating) {
        if (optionType == Option::Call) {
            extreme = *std::min_element(St.begin(), St.end());
        } else { // Put
            extreme = *std::max_element(St.begin(), St.end());
        }
    } else {
        // For fixed strike, follow the standard: max for call, min for put.
        if (optionType == Option::Call) {
            extreme = *std::max_element(St.begin(), St.end());
        } else { // Put
            extreme = *std::min_element(St.begin(), St.end());
        }
    }

  return extreme;
}
double PriceClass::calculateP_StratifiedSampling(const Option& option, double S, double r, double sigma, double T, unsigned int nSimulations, unsigned int mStrata){
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        return 0.0;
    }

    double price = 0.0;
    static std::random_device rd; 
    static std::mt19937 gen(rd());
    for (unsigned int i = 0; i < mStrata; ++i){
        double iPayoff = 0.0; 
        double start = static_cast<double>(i) / mStrata;
        double end = static_cast<double>(i + 1) / mStrata;
        std::uniform_real_distribution<double> dist(start, end);
        unsigned int simsPerStratum = nSimulations / mStrata;
        
        for (unsigned int j = 0; j < simsPerStratum; ++j){             
            double randUniform = dist(gen);
            double randomterm = sigma * std::sqrt(lookbackOption->getExpiry()) * normal_ppf(randUniform);
            double ST = S * std::exp((r - 0.5 * sigma * sigma) * lookbackOption->getExpiry() + randomterm);
            double extreme = get_extreme(S, r, sigma, ST, T, 13, lookbackOption->getType(), lookbackOption->getStrikeType());
            double jPayoff;
            if (lookbackOption->getStrikeType() == LookbackOption::Floating) {
                jPayoff = lookbackOption->payoff(ST, extreme);
            } else {
                jPayoff = option.payoff(extreme);
            }
            iPayoff += jPayoff / static_cast<double>(simsPerStratum);
        }
        price += iPayoff / mStrata;
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
    const LookbackOption *lookbackOption = dynamic_cast<const LookbackOption *>(&option);
    if (!lookbackOption) {
        return 0.0;
    }

    double dt = lookbackOption->getExpiry() / lookbackOption->getPeriods();
    std::vector<double> payoffs(nSimulations);
    std::vector<double> ST(nSimulations);
    std::vector<double> adjustedPayoffs(nSimulations);
    std::normal_distribution<double> dist(0.0, 1.0);

    for (unsigned int i = 0; i < nSimulations; ++i){
        double spotPath = S;
        double extreme = S; 
        for (unsigned int j = 0; j < lookbackOption->getPeriods(); ++j) {
            if (j > 0) {
                double randNormal = dist(AssetHistory::get_random_generator());
                spotPath *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * randNormal);
            }
            extreme = updateExtreme(extreme, spotPath, lookbackOption);
        }
        ST[i] = spotPath;
        if (lookbackOption->getStrikeType() == LookbackOption::Floating) {
            payoffs[i] = lookbackOption->payoff(spotPath, extreme);
        } else {
            payoffs[i] = option.payoff(extreme);
        }
    }   
    double cov = covariance(payoffs, ST);
    double var = covariance(ST, ST);
    double c = cov / var;
    double ST_bar = std::accumulate(ST.begin(), ST.end(), 0.0) / ST.size();
    std::vector<double> adjusted(nSimulations);
    for (unsigned int i = 0; i < nSimulations; ++i){
        adjusted[i] = payoffs[i] - c * (ST[i] - ST_bar);         
    } 

    double price = std::exp(-r * lookbackOption->getExpiry()) *
                   std::accumulate(adjusted.begin(), adjusted.end(), 0.0) / adjusted.size();
    return price;
}
