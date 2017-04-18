cmake_minimum_required(VERSION 3.7)

set(CMAKE_VERBOSE_MAKEFILE 0)

project(irclog2json CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=gnu++14 -DPICOJSON_USE_INT64")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)


find_package(ICU)
include_directories(${ICU_INCLUDE_DIRS})
link_directories(${ICU_LIBRARIES})

include_directories(vendor/picojson)

set(target irclog2json)
add_executable(${target}
  irclog2json.cpp
  converter.cpp
  message/line_converter_base.cpp
  message/tiarra_log_line_converter.cpp
  message/madoka_log_line_converter.cpp
  message/message_base.cpp
  message/basic_message.cpp
  message/join.cpp
  message/kick.cpp
)
