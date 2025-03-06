# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(IB9J0_Group_2)

# Set C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

#Add source files
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp 
              ${CMAKE_SOURCE_DIR}/src/LookbackOption.cpp 
              ${CMAKE_SOURCE_DIR}/src/Option.cpp 
              ${CMAKE_SOURCE_DIR}/src/PriceClass.cpp
              ${CMAKE_SOURCE_DIR}/src/AssetHistory.cpp
              ${CMAKE_SOURCE_DIR}/src/Util.cpp)


add_executable(my_program ${SRC_FILES})
target_include_directories(my_program PRIVATE ${CMAKE_SOURCE_DIR}/src)


####################### TESTING STUFF STARTS HERE ###########################################################

enable_testing()
add_subdirectory(tests)

