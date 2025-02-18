//AssetHistory provided in market-sim-demo
//Some edits to the accessibility of the random genreator function
#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

/// <summary>
/// 
/// This class is responsible for reading and writing/generating asset price histories to 
/// a file.
/// 
/// </summary>
class AssetHistory
{
	// prevent objects of this class by making constructor private 
	AssetHistory();


public:

	/// <summary>
	/// 
	/// Generate the log price history of m assets where the logarithm 
	/// of the price is a brownian motion with drift.
	/// At time t, ln(price) ~ N(mu*t, sigma^2*t)
	/// 
	/// filename - file to store the prices
	/// asset_names - name of each asset
	/// mu - array of means (drift)
	/// sigma - array of volatilities (scale)
	/// n - number of history points
	/// 
	/// </summary>
	static void generate_log_price_history(const std::string& filename, const std::vector<std::string>& asset_names, double* mu, double* sigma, unsigned int n);

	/// <summary>
	/// 
	/// Read price history of all assets from a file at all timesteps
	/// 
	/// filename - file where prices are stored
	/// asset_names - name of each asset to restore from the file
	/// data - array to store the price histories, the size must be at least m_max * n_max
	/// m_max - maximum number of assets
	/// n_max - maximum number of history points
	/// 
	/// </summary>
	static void read_price_history(const std::string& filename, std::vector<std::string>& asset_names, double** const data, unsigned int m_max, unsigned int n_max);

	/// test the functions by generating asset history and then restoring it from file
	static void test();

    // Accessor for the random generator
    // Makes it safer to 
    static std::default_random_engine& get_random_generator();

private:
    // Used to generate random numbers (declared only once)
    thread_local static std::default_random_engine RANDOM_GENERATOR;


};
