include_directories(${PROJECT_SOURCE_DIR})
include_directories(../vendor/doctest)

set(test_target irclog2json_test)
add_executable(${test_target}
  irclog2json_test.cpp

  tiarra/privmsg.cpp
  tiarra/notice.cpp
  tiarra/topic.cpp
  tiarra/mode.cpp
  tiarra/nick.cpp
  tiarra/part.cpp
  tiarra/quit.cpp
  tiarra/kick.cpp
  tiarra/join.cpp
)
target_link_libraries(${test_target}
  ${target_common_lib}
  ${ICU_LIBRARIES}
)

add_test(irclog2json irclog2json_test)
