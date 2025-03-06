# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(IB9J0_Group_2)

# Set C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)
add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")


#Add source files
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp 
              ${CMAKE_SOURCE_DIR}/src/LookbackOption.cpp 
              ${CMAKE_SOURCE_DIR}/src/Option.cpp 
              ${CMAKE_SOURCE_DIR}/src/PriceClass.cpp
              ${CMAKE_SOURCE_DIR}/src/AssetHistory.cpp
              ${CMAKE_SOURCE_DIR}/src/Util.cpp)


add_executable(my_program ${SRC_FILES})
target_include_directories(my_program PRIVATE ${CMAKE_SOURCE_DIR}/src)


#//////////////////////////////////////////////////////////////////////////////
#               lookback_convergence 
#/////////////////////////////////////////////////////////////////////////////
add_executable(lookback_convergence
    src/LookbackConvergence.cpp
    src/Option.cpp
    src/LookbackOption.cpp
    src/PriceClass.cpp
    src/Util.cpp
    src/AssetHistory.cpp 
)
target_include_directories(lookback_convergence
    PUBLIC    ${CMAKE_SOURCE_DIR}/src
)

#//////////////////////////////////////////////////////////////////////////////
#               generate_lookback_output 
#/////////////////////////////////////////////////////////////////////////////

add_executable(generate_lookback_output
    src/Option.cpp
    src/LookbackOption.cpp
    src/PriceClass.cpp
    src/Util.cpp
    src/AssetHistory.cpp
    src/GenerateLookback_output.cpp
)
target_include_directories(generate_lookback_output PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}"  # Adjust this path to where capture_output.hpp actually resides
)


####################### TESTING STUFF STARTS HERE ###########################################################

enable_testing()
add_subdirectory(tests)

