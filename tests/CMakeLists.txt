cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(UnitTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

INCLUDE_DIRECTORIES(../)

if (MSVC)
    add_compile_options(/W4) # /WX
else()
    add_compile_options(-Wall -Wextra -pedantic) # -Werror
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(${PROJECT_NAME}
        UnitTests.cpp
        ../FuzzyControl.hpp
)
include(CTest)
enable_testing()

gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        gtest
        gtest_main
        gmock
)