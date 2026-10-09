cmake_minimum_required(VERSION 3.20)
project(lld_cpp_interview_prep LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(LLD_ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
option(LLD_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
set(LLD_PRACTICE_EXAMPLE "" CACHE STRING "Question directory to build as practice_tests")

find_package(Threads REQUIRED)
add_library(lld_project_options INTERFACE)
target_compile_features(lld_project_options INTERFACE cxx_std_11)
target_include_directories(lld_project_options INTERFACE ${PROJECT_SOURCE_DIR}/common)

if(MSVC)
    target_compile_options(lld_project_options INTERFACE /W4 /permissive-)
    if(LLD_WARNINGS_AS_ERRORS)
        target_compile_options(lld_project_options INTERFACE /WX)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lld_project_options INTERFACE
        -Wall -Wextra -Wpedantic -Wconversion -Wshadow
    )
    if(LLD_WARNINGS_AS_ERRORS)
        target_compile_options(lld_project_options INTERFACE -Werror)
    endif()
    if(LLD_ENABLE_SANITIZERS)
        target_compile_options(lld_project_options INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
        target_link_options(lld_project_options INTERFACE -fsanitize=address,undefined)
    endif()
endif()

enable_testing()
add_executable(optional_tests common/optional_tests.cpp)
target_link_libraries(optional_tests PRIVATE lld_project_options)
add_test(NAME optional_tests COMMAND optional_tests)
add_subdirectory(solutions/01-parking-lot)

function(lld_add_example directory target)
    add_library(${target} INTERFACE)
    target_include_directories(${target} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/solutions/${directory})
    target_link_libraries(${target} INTERFACE lld_project_options Threads::Threads)
    add_executable(${target}_tests solutions/${directory}/tests.cpp)
    target_include_directories(${target}_tests PRIVATE common)
    target_link_libraries(${target}_tests PRIVATE ${target})
    add_test(NAME ${target}_tests COMMAND ${target}_tests)
endfunction()

lld_add_example(02-connect-four connect_four)
lld_add_example(03-amazon-locker amazon_locker)
lld_add_example(04-elevator elevator)
lld_add_example(05-file-system file_system)
lld_add_example(06-movie-ticket-booking movie_ticket_booking)
lld_add_example(07-logging-service logging_service)
lld_add_example(08-rate-limiter rate_limiter)
lld_add_example(09-inventory-management inventory_management)

set(LLD_EXAMPLES
    01-parking-lot
    02-connect-four
    03-amazon-locker
    04-elevator
    05-file-system
    06-movie-ticket-booking
    07-logging-service
    08-rate-limiter
    09-inventory-management
)
if(LLD_PRACTICE_EXAMPLE)
    list(FIND LLD_EXAMPLES "${LLD_PRACTICE_EXAMPLE}" practice_index)
    if(practice_index EQUAL -1)
        message(FATAL_ERROR "Unknown LLD_PRACTICE_EXAMPLE: ${LLD_PRACTICE_EXAMPLE}")
    endif()
    if(LLD_PRACTICE_EXAMPLE STREQUAL "01-parking-lot")
        set(practice_source solutions/01-parking-lot/tests/parking_lot_test.cpp)
    else()
        set(practice_source solutions/${LLD_PRACTICE_EXAMPLE}/tests.cpp)
    endif()
    add_executable(practice_tests ${practice_source})
    target_compile_definitions(practice_tests PRIVATE LLD_PRACTICE=1)
    target_include_directories(practice_tests PRIVATE common questions/${LLD_PRACTICE_EXAMPLE})
    target_link_libraries(practice_tests PRIVATE lld_project_options Threads::Threads)
    add_test(NAME practice_tests COMMAND practice_tests)
endif()
