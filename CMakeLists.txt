cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(FuzzyControl LANGUAGES C CXX)
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4) # /WX
else()
    add_compile_options(-Wall -Wextra -pedantic) # -Werror
endif()

add_executable(AutomatedSnowmaker
        AutomatedSnowmaker.cpp
        FuzzyControl.hpp
)