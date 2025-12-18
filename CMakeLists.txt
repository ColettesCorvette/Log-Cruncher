cmake_minimum_required(VERSION 3.30)
project(lcruncher VERSION 0.2 LANGUAGES CXX)

# --- CONFIGURATION ---
set(SOURCES
    src/main.cpp
    src/Analyzer.cpp
    src/MMapLoader.cpp
)

set(HEADERS
    include/Analyzer.hpp
    include/MMapLoader.hpp
)

add_executable(lcruncher ${SOURCES} ${HEADERS} benchmarks/timer.cpp)

target_include_directories(lcruncher PRIVATE include)
target_compile_features(lcruncher PRIVATE cxx_std_17)

# --- OPTIMISATIONS ---

if(MSVC)
    target_compile_options(lcruncher PRIVATE /W4)
else()
    target_compile_options(lcruncher PRIVATE -Wall -Wextra -Werror -Wpedantic)
endif()

# --- AVX/AVX2 ---
if(NOT MSVC)
    target_compile_options(lcruncher PRIVATE -march=native)
    # for github's potato servers, so that it does not trigger "Wait avx512 insctuctions unsupported, I'll fail the build"
    target_compile_options(lcruncher PRIVATE -mavx512f -mavx512bw -mavx512dq -Wno-psabi)
endif()

# --- Link Time Optimization ---
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)

if(result)
    set_property(TARGET lcruncher PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "IPO / LTO enabled")
else()
    message(WARNING "IPO / LTO not supported: ${output}")
endif()

