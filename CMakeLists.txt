cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE 0)

project(tiarra2json CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=gnu++14 -DPICOJSON_USE_INT64")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s -DNDEBUG")

include_directories(vendor/picojson)

set(target tiarra2json)
add_executable(${target}
  tiarra2json.cpp
  converter.cpp
  message/factory.cpp
  message/message_base.cpp
  message/privmsg.cpp
  message/notice.cpp
  message/nick.cpp
  message/join.cpp
  message/part.cpp
  message/quit.cpp
  message/kick.cpp
  message/topic.cpp
)
