#pragma once
#include <cstdlib>
#include <stdexcept>
#include <string>

void capture_output(const std::string &command, const std::string &outputFile) {
    std::string fullCommand = command + " > \"" + outputFile + "\" 2>&1";
    if (std::system(fullCommand.c_str()) != 0)
        throw std::runtime_error("Command failed: " + command);
}