cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE 0)

project(irclog2json CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=gnu++14 -DPICOJSON_USE_INT64")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

include_directories(vendor/picojson)

set(target irclog2json)
add_executable(${target}
  irclog2json.cpp
  converter.cpp
  message/factory.cpp
  message/message_base.cpp
  message/basic_message.cpp
  message/join.cpp
  message/kick.cpp
)
