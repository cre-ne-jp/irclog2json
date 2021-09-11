include_directories(${PROJECT_SOURCE_DIR})
include_directories(../vendor/doctest)

set(test_target irclog2json_test)
add_executable(${test_target}
  irclog2json_test.cpp

  tiarra/privmsg.cpp
  tiarra/notice.cpp
  tiarra/topic.cpp
  tiarra/nick.cpp
  tiarra/part.cpp
  tiarra/quit.cpp
  tiarra/kick.cpp
  tiarra/join.cpp

  tiarra/mode.cpp

  madoka/privmsg.cpp
  madoka/notice.cpp
  madoka/topic.cpp
  madoka/nick.cpp
  madoka/part.cpp
  madoka/quit.cpp
  madoka/kick.cpp
  madoka/join.cpp

  madoka/auto_down.cpp
  madoka/auto_down_nick_set.cpp
  madoka/mode.cpp
  madoka/time_tone.cpp

  madoka_iso-2022-jp/privmsg.cpp
  madoka_iso-2022-jp/topic.cpp
  madoka_iso-2022-jp/part.cpp
  madoka_iso-2022-jp/quit.cpp
  madoka_iso-2022-jp/kick.cpp

  madoka_iso-2022-jp/time_tone.cpp
)
target_link_libraries(${test_target}
  ${target_common_lib}
  ${ICU_LIBRARIES}
)

add_test(irclog2json irclog2json_test)
