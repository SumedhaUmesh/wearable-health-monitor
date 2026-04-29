# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wearable_health_monitor LANGUAGES CXX C)

target_sources(app PRIVATE app/main.cpp)
