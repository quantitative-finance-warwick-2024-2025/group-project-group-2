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
   ```
   mkdir build
   cd build
   ```

4. **Generater Build Files with CMake** 
    ```
    cmake ..
    ```

5. **Compile the Project**
    ```
    cmake --build
    ```

### Running the Main Application

1. **Navigate to the Main Application's Build Folder**
    ```
    cd build
    ```

2. **Run the Executable**
On Mac:
    ```
    ./lookback_convergence
    ```

On Windows:
    ```
    lookback_convergence.exe
    ```



### Running the Tests

1. **Navigate to the Tests Build Folder**
    ```
    cd build/tests
    ```
2. **Run the Executable**
On Mac:
    ```
    ./TestOption
    ./TestLookbackOption
    ./TestPriceClass
    ```

On Windows:

    ```
    TestOption.exe, 
    TestLookbackOption.exe, 
    TestPriceClass.exe
    ```
    

***

## Implementation

The UML below briefly outlines the design of our codebase.

![](./assets/UML.png)

Below is a concise overview of the project’s class structure and supporting modules, as depicted in the UML diagram:

- **Option**  
  Serves as an abstract base class for financial derivatives. It declares core attributes such as the strike price, expiry, and option type, and provides a pure virtual function, `payoff()`, to be implemented by derived classes.

- **LookbackOption**  
  Inherits from `Option` and specialises the payoff logic to account for the extreme (maximum or minimum) asset price observed over the option’s lifetime. It introduces additional parameters, including the number of monitoring periods and whether the strike is fixed or floating.

- **PriceClass**  
  Implements various Monte Carlo simulation methods (e.g., naive, antithetic, control variates, and stratified sampling) for pricing options. It calls the option’s `payoff()` function to compute path-dependent payoffs and ultimately determine the option price.

- **AssetHistory and Util**  
  Provide utility functions and classes for reading, generating, and manipulating asset price data. These modules operate independently from the option-pricing classes but support the simulation process.

- **Test Suite**  
  Includes test files such as `TestPriceClass`, `TestOption`, and `TestLookbackOption` that verify the correctness of each component, ensuring robust functionality across the entire codebase.


Overall, this design fosters modularity and extensibility: each class or module focuses on a specific task — whether it is payoff calculation, simulation, or data handling — while the test suite and analysis scripts ensure correctness and facilitate deeper investigation of pricing behaviour.



