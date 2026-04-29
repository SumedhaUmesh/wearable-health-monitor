# SPDX-License-Identifier: Apache-2.0
# Host-only unit tests (GoogleTest). Build with:
#   cmake -S tests -B build-tests -G Ninja && cmake --build build-tests && ctest --test-dir build-tests

cmake_minimum_required(VERSION 3.20)
project(wearable_host_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  wearable_host_tests
  test_iir_sos.cpp
  test_heart_rate.cpp
  test_activity.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/algorithms/heart_rate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/algorithms/activity.cpp
)

target_include_directories(wearable_host_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(wearable_host_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(wearable_host_tests)
