#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/madoka_line_parser.h"
#include "message/message_base.h"

#include "tests/test_helper.h"

// Madokaの場合はPRIVMSGとNOTICEとで判定条件がほぼ同じなので、
// 見分けられないかもしれない
TEST_CASE("Madoka NOTICE") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineParser parser{"もの書き", tm_date};
  const auto m =
      parser.ToMessage("12:34:56 (#もの書き:Toybox) FetchTitle: cre");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "NOTICE");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 12:34:56 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "FetchTitle: cre");
  }
}

TEST_CASE("Madoka NOTICE containing mIRC codes") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineParser parser{"もの書き", tm_date};
  const auto m = parser.ToMessage(
      "12:34:56 (#もの書き:Toybox) "
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0B\x0C\x0E\x0F"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
      "通常の文字");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  CHECK_OBJ_STR_EQ(o, "message",
                   "\x02\x03\x04\x0F\x11\x16\x1D\x1E\x1F通常の文字");
}
