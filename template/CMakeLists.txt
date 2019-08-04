cmake_minimum_required(VERSION 3.8.2)

project(ArduinoTestingTemplate VERSION 1.0
  DESCRIPTION "GOS Arduino Testing Template"
  LANGUAGES C CXX ASM)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/.cmake")

include(Arduino-Toolchain)
  
arduino_cmake_project(ArduinoTestingTemplate BOARD_NAME uno)

add_arduino_executable(ArduinoTestingTemplate template.ino)

set_target_upload_port(ArduinoTestingTemplate COM3)

message(STATUS "")
message(STATUS "BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_MODULE_PATH    : ${CMAKE_MODULE_PATH}")
message(STATUS "  Compiler ID          : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests          : ${FDS_PACKAGE_TESTS}")
message(STATUS "  Force Shared CRT     : ${gtest_force_shared_crt}")
#message(STATUS "  Boost version        : ${Boost_VERSION}")
#message(STATUS "  Boost include dir    : ${Boost_INCLUDE_DIRS}")
message(STATUS "")
