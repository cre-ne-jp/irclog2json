#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka KICK with message") {
  using irclog2json::message::MadokaLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLogLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("08:54:52 - ocha by Toybox from #もの書き (ごめんね。)");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "KICK");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 08:54:52 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox");
  }

  SUBCASE("target") {
    CHECK_OBJ_STR_EQ(o, "target", "ocha");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "ごめんね。");
  }
}