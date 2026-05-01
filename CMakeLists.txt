cmake_minimum_required(VERSION 3.16)

project(NumericalFinanceProject VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE RANDOM_GENERATOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomGenerator/src/*.cpp"
)

file(GLOB_RECURSE PDE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/PDE/src/*.cpp"
)

file(GLOB_RECURSE SDE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/SDE/src/*.cpp"
)

add_executable(numerical_finance
    main.cpp
    ${RANDOM_GENERATOR_SOURCES}
    ${PDE_SOURCES}
    ${SDE_SOURCES}
)

target_include_directories(numerical_finance PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomGenerator/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomGenerator/include/UniformGenerator"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomGenerator/include/ContinuousGenerator"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomGenerator/include/DiscreteGenerator"
    "${CMAKE_CURRENT_SOURCE_DIR}/PDE/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDE/include"
)

option(ENABLE_TEST_GENERATOR "Enable test-only code in main.cpp" OFF)
if(ENABLE_TEST_GENERATOR)
    target_compile_definitions(numerical_finance PRIVATE TEST)
endif()
