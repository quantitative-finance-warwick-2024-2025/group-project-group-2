#include <filesystem>
#include <iostream>
#include "capture_output.hpp"

int main()
{
    const std::filesystem::path root = PROJECT_ROOT_DIR;
    
    // Path to the lookback_convergence executable
    const std::filesystem::path cmd = root / "build" / "lookback_convergence";
    
    // Output CSV file inside the src folder
    const std::filesystem::path output = root / "src" / "lookback_results.csv";
    
    capture_output(cmd.string(), output.string());
    return 0;
}


// #include <cstdlib>
// #include <iostream>
// #include <string>

// int main() {
//     std::string cmd = "./lookback_convergence > ../src/lookback_results.csv 2>&1";

//     int ret = std::system(cmd.c_str());
//     if (ret != 0) {
//         std::cerr << "Failed to run lookback_convergence, error code: " << ret << "\n";
//         return ret;
//     }
//     return 0;
// }
