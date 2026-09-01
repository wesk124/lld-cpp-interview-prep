cmake_minimum_required(VERSION 3.20)
project(lld_cpp_interview_prep LANGUAGES CXX)

option(LLD_ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

add_library(lld_project_options INTERFACE)
target_compile_features(lld_project_options INTERFACE cxx_std_20)

if(MSVC)
    target_compile_options(lld_project_options INTERFACE /W4 /permissive-)
else()
    target_compile_options(lld_project_options INTERFACE
        -Wall -Wextra -Wpedantic -Wconversion -Wshadow
    )
endif()

if(LLD_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(lld_project_options INTERFACE -fsanitize=address,undefined)
    target_link_options(lld_project_options INTERFACE -fsanitize=address,undefined)
endif()

enable_testing()
add_subdirectory(solutions/01-parking-lot)
