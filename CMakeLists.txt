cmake_minimum_required(VERSION 3.8)

set(CMAKE_VERBOSE_MAKEFILE 0)

project(irclog2json CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -DPICOJSON_USE_INT64")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

include_directories(vendor/picojson)

find_package(PkgConfig)
pkg_check_modules(ICU REQUIRED icu-uc)
include_directories(${ICU_INCLUDE_DIRS})
link_directories(${ICU_LIBRARY_DIRS})

set(target_common_lib irclog2json-common)
add_library(${target_common_lib}
  STATIC

  message/utf8_line_converter.cpp
  message/iso_2022_jp_line_converter.cpp

  message/tiarra_log_line_converter.cpp
  message/tiarra_iso_2022_jp_line_converter.cpp
  message/tiarra_re.cpp

  message/madoka_log_line_converter.cpp
  message/madoka_iso_2022_jp_line_converter.cpp
  message/madoka_re.cpp

  message/message_base.cpp
  message/basic_message.cpp
  message/join.cpp
  message/kick.cpp

  converter.cpp
)

set(target irclog2json)
add_executable(${target}
  irclog2json.cpp
)

target_link_libraries(${target}
  ${target_common_lib}
  ${ICU_LIBRARIES}
)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif(BUILD_TESTS)
