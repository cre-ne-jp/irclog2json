#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/tiarra_line_parser.h"

#include "tests/test_helper.h"

TEST_CASE("Tiarra JOIN from FQDN") {
  using irclog2json::message::TiarraLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  TiarraLineParser parser{"もの書き", tm_date};
  const auto m = parser.ToMessage(
      "22:21:16 + Toybox_ (Toybox_!dicebot@services.cre.jp) to #もの書き@cre");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "JOIN");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 22:21:16 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox_");
  }

  SUBCASE("user") {
    CHECK_OBJ_STR_EQ(o, "user", "dicebot");
  }

  SUBCASE("host") {
    CHECK_OBJ_STR_EQ(o, "host", "services.cre.jp");
  }

  SUBCASE("messageが存在しない") {
    CHECK(o.find("message") == o.end());
  }
}

TEST_CASE("Tiarra JOIN from IP") {
  using irclog2json::message::TiarraLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  TiarraLineParser parser{"もの書き", tm_date};
  const auto m = parser.ToMessage(
      "22:21:16 + Toybox_ (Toybox_!dicebot@192.168.0.1) to #もの書き@cre");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "JOIN");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 22:21:16 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox_");
  }

  SUBCASE("user") {
    CHECK_OBJ_STR_EQ(o, "user", "dicebot");
  }

  SUBCASE("host") {
    CHECK_OBJ_STR_EQ(o, "host", "192.168.0.1");
  }

  SUBCASE("messageが存在しない") {
    CHECK(o.find("message") == o.end());
  }
}
