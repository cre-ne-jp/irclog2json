#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/madoka_line_parser.h"
#include "message/message_base.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka NICK from user") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineParser parser{"cre", tm_date};
  const auto m = parser.ToMessage("12:34:56 ocha -> ocha[away]");

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

TEST_CASE("Madoka NICK from self") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineParser parser{"cre", tm_date};
  const auto m = parser.ToMessage("14:12:46 [!] nick changed (dice2 -> dice)");

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
    CHECK_OBJ_STR_EQ(o, "nick", "dice2");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "dice");
  }
}
