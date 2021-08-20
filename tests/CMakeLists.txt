include_directories(${PROJECT_SOURCE_DIR})
include_directories(../vendor/doctest)

set(test_target irclog2json_test)
add_executable(${test_target}
  irclog2json_test.cpp
  tiarra_messages.cpp

  ../message/line_converter_base.cpp
  ../message/tiarra_log_line_converter.cpp
  ../message/madoka_log_line_converter.cpp
  ../message/iso_2022_jp_line_converter.cpp
  ../message/message_base.cpp
  ../message/basic_message.cpp
  ../message/join.cpp
  ../message/kick.cpp
)
target_link_libraries(${test_target} ${ICU_LIBRARIES})

add_test(irclog2json irclog2json_test)
