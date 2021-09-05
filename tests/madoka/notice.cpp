#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

// Madokaの場合はPRIVMSGとNOTICEとで判定条件がほぼ同じなので、
// 見分けられないかもしれない
TEST_CASE("Madoka NOTICE") {
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("12:34:56 (#もの書き:Toybox) FetchTitle: cre");

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
