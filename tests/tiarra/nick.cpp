#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/tiarra_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Tiarra NICK from user") {
  using irclog2json::message::TiarraLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  TiarraLogLineConverter converter{"cre", tm_date};
  const auto m = converter.ToMessage("12:34:56 ocha -> ocha[away]");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "NICK");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 12:34:56 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "ocha");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "ocha[away]");
  }
}

TEST_CASE("Tiarra NICK from self") {
  using irclog2json::message::TiarraLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  TiarraLogLineConverter converter{"cre", tm_date};
  const auto m = converter.ToMessage("14:12:46 My nick is changed (Toybox => Toybox_dead)");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "NICK");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 14:12:46 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "Toybox_dead");
  }
}
