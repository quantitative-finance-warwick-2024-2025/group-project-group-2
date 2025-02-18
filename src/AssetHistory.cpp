//AssetHistory provided in market-sim-demo

#include "AssetHistory.h"
#include "Util.h"

// random seed is based on the current time
thread_local std::default_random_engine AssetHistory::RANDOM_GENERATOR{static_cast<unsigned int>(time(0)) };

using stringstream = std::stringstream;

// Define accessor for the random generator
std::default_random_engine& AssetHistory::get_random_generator()
{
    return RANDOM_GENERATOR;
}

// format the contents of array arr of double to stringstream ss
// contents are comma seperated
void format_line(const double* const arr, stringstream& ss, unsigned int m)
{
	for (unsigned int i = 0; i < m - 1; ++i)
	{
		ss << arr[i] << ",";
	}

	ss << arr[m - 1] << "\n";
}

// read the contents of a comma seperated line and extract m numbers
void parse_line(double* const arr, const std::string& s, unsigned int m)
{
	stringstream ss(s);
	unsigned int i = 0;
	std::string x;

	while (i < m && ss.good())
	{
			std::getline(ss, x, ',');
			arr[i] = strtod(x.data(), NULL);
			++i;
	}
}

// read the contents of a comma seperated line and extract an unknown amount of numbers
void parse_line(std::vector<double>& arr, const std::string& s)
{
	stringstream ss(s);
	std::string x;

	while (ss.good())
	{
		std::getline(ss, x, ',');
		arr.push_back(strtod(x.data(), NULL));
	}
}

// create a formatted header ready to write to file from the array arr
void format_header(const std::vector<std::string>& arr, stringstream& ss)
{
	for (unsigned int i = 0; i < arr.size() - 1; ++i)
	{
		ss << arr[i] << ",";
	}

	ss << arr[arr.size() - 1] << "\n";
}

// read a formatted header from string s and store each comma seperated string in arr
void parse_header(std::vector<std::string>& arr, const std::string& s)
{
	stringstream ss(s);
	std::string x;

	while (ss.good())
	{
		std::getline(ss, x, ',');
		arr.push_back(x.data());
	}
}

void AssetHistory::generate_log_price_history(const std::string& filename, const std::vector<std::string>& asset_names, double* mu, double* sigma, unsigned int n)
{
	// the number of assets to generate for
	unsigned int m = asset_names.size();

	// open the file stream
	std::ofstream file(filename);

	// storage for output lines
	stringstream line;

	// storage for data at each time
	std::vector<double> timestep_data(m);

	// object to generate Z where Z ~ N(0, 1)
	std::normal_distribution<double> dist(0.0, 1.0);

	double ti = 0.0, Zi = 0.0;

	// write the header to file containing the asset names
	format_header(asset_names, line);
	file << line.str();

	for (unsigned int i = 0; i < n; ++i)
	{
		for (unsigned int j = 0; j < m; ++j)
		{
			Zi += dist(RANDOM_GENERATOR);

			// unit time increments
			ti = i;

			// obtain ln(price) where ln(price) ~ N(mu*t, sigma^2*t) from Z
			timestep_data[j] = ti * mu[j] + sigma[j] * Zi;
		}

		// empty the string stream
		line.str(std::string());
		line.clear();
		// load the data into the line
		format_line(timestep_data.data(), line, timestep_data.size());

		// write the line to file
		file << line.str();
	}

	file.close();
}

void AssetHistory::read_price_history(const std::string& filename, std::vector<std::string>& asset_names, double** const data, unsigned int m_max, unsigned int n_max)
{
	// open the file stream
	std::ifstream file(filename);

	// input lines
	std::string line;

	// process the header
	std::getline(file, line);
	asset_names.resize(0);
	parse_header(asset_names, line);

	// determine how many assets to retrieve
	unsigned int m = std::min<unsigned int>(m_max, asset_names.size());

	unsigned int i = 0;

	while (i < n_max && file >> line)
	{
		parse_line(data[i], line, m);
		++i;
	}

	file.close();
}