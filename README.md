# IB9JHO - Programming for Quantitative Finance - Group Project (C++)

***

## Introduction
 Lookback options derive their value from the extreme asset prices observed during the option's lifetime—a characteristic that precludes standard closed-form pricing methods. Our approach leverages a rigorous mathematical framework based on geometric Brownian motion to model asset dynamics and derives the corresponding payoff structures for both fixed and floating strike options. In addition, we implement multiple variance reduction techniques to enhance the accuracy and efficiency of our simulations.

***

## Project Overview

This project is structured to ensure modularity, extensibility, and maintainability:
- **Option and LookbackOption Classes:** Define the base and specialised option types. The `LookbackOption` class implements payoff functions that capture the path dependency by tracking extreme asset prices.
- **PriceClass:** Encapsulates the Monte Carlo simulation methods (naïve, antithetic variates, control variates, and stratified sampling) to compute option prices.
- **AssetHistory and Util Modules:** Support functions for generating and processing asset price histories and auxiliary operations.
- **Testing:** A comprehensive suite of unit tests (using Catch2) ensures that each component functions as expected.
- **Analysis Module:** Generates CSV outputs for further analysis and visualization.

***

## Instructions to Run the Project

### Prerequisites
- A C++ compiler
- CMake version 3.14 or later

###  Project Structure

- **`CMakeLists.txt`**: Top-level build configuration.
- **`main.cpp`**: Entry point for the main application.
- **`Option.h` / `Option.cpp`**: Defines an abstract base class for options.
- **`LookbackOption.h` / `LookbackOption.cpp`**: Implements path-dependent logic for lookback options.
- **`PriceClass.h` / `PriceClass.cpp`**: Houses various Monte Carlo pricing methods.
- **`GenerateLookback_output.cpp` / `LookbackConvergence.cpp`**: Contains code for outputting CSV files used in further analysis.
- **`AssetHistory.cpp` / `AssetHistory.h`**: Simualtion of a market e.g path of underlying stocks using GBM
- **`Util.cpp` / `Util.h`**: Contains helper functions for mathematical operations
- **`tests/`**: Contains Catch2-based unit tests.


### Building the Project

Below is a step-by-step guide for building the project via the command line. If you are using Visual Studio Code with the CMake Tools extension, you can adapt these steps accordingly.

1. **Clone or Download** the repository to your local machine.

2. **Open a Terminal** in the project’s root directory (where the main `CMakeLists.txt` file is located).

3. **Create a Build Directory** and navigate into it:
   ```bash
   mkdir build
   cd build

4. **Generater Build Files with CMake** 
    ```bash
    cmake ..

5. **Compile the Project**
    ```bash
    cmake --build

### Running the Main Application

1. **Navigate to the Main Application's Build Folder**
    ```bash
    cd build

2. **Run the Executable**
On Mac:
    ``bash
    ./generate_lookback_output

On Windows:
    ``bash
    generate_lookback_output.exe


### Running the Tests

1. **Navigate to the Tests Build Folder**
    ```bash
    cd build/tests

2. **Run the Executable**
On Mac:
    ``bash
    ./TestOption
    ./TestLookbackOption
    ./TestPriceClass

On Windows:
    ``bash
    TestOption.exe
    TestLookbackOption.exe
    TestPriceClass.exe






