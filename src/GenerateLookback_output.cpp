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
