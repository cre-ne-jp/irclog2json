#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka TOPIC") {
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date{};

  strptime("1999-02-21", "%F", &tm_date);

  MadokaLineConverter converter{"kataribe", tm_date};
  const auto m = converter.ToMessage("00:38:04 Topic of channel #kataribe by sf: 創作TRPG語り部関係雑談:質問者・相談者歓迎(MLに全転送)");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "TOPIC");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "kataribe");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "1999-02-21 00:38:04 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "sf");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "創作TRPG語り部関係雑談:質問者・相談者歓迎(MLに全転送)");
  }
}
