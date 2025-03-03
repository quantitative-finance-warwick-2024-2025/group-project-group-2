set(TEST_SRC_FILES 
    ${CMAKE_SOURCE_DIR}/src/LookbackOption.cpp 
    ${CMAKE_SOURCE_DIR}/src/Option.cpp 
    ${CMAKE_SOURCE_DIR}/src/PriceClass.cpp
    ${CMAKE_SOURCE_DIR}/src/AssetHistory.cpp
    ${CMAKE_SOURCE_DIR}/src/Util.cpp
)

add_executable(TestOption ${TEST_SRC_FILES} TestOption.cpp)
target_include_directories(TestOption PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/extern/catch2)
add_test(NAME TestOption COMMAND TestOption)

add_executable(TestLookbackOption ${TEST_SRC_FILES} TestLookbackOption.cpp)
target_include_directories(TestLookbackOption PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/extern/catch2)
add_test(NAME TestLookbackOption COMMAND TestLookbackOption)

add_executable(TestPriceClass ${TEST_SRC_FILES} TestPriceClass.cpp)
target_include_directories(TestPriceClass PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/extern/catch2)
add_test(NAME TestPriceClass COMMAND TestPriceClass)
