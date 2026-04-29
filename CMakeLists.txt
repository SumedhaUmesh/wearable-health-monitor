# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wearable_health_monitor LANGUAGES CXX C)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_sources(
  app
  PRIVATE
    app/main.cpp
    src/drivers/max30102_driver.cpp
    src/drivers/mock_ppg_driver.cpp
    src/drivers/imu_skeleton_driver.cpp
    src/drivers/mock_imu_driver.cpp
    src/services/sensor_manager.cpp
    src/algorithms/heart_rate.cpp
    src/algorithms/activity.cpp
)
