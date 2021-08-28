#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka JOIN from FQDN") {
  using irclog2json::message::MadokaLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLogLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("05:00:17 + dice2(~dice@sv1.trpg.net) to #もの書き");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "JOIN");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 05:00:17 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "dice2");
  }

  SUBCASE("user") {
    CHECK_OBJ_STR_EQ(o, "user", "dice");
  }

  SUBCASE("host") {
    CHECK_OBJ_STR_EQ(o, "host", "sv1.trpg.net");
  }

  SUBCASE("messageが存在しない") {
    CHECK(o.find("message") == o.end());
  }
}

TEST_CASE("Madoka JOIN from IP") {
  using irclog2json::message::MadokaLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLogLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("05:00:17 + dice2(~dice@192.168.0.1) to #もの書き");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "JOIN");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 05:00:17 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "dice2");
  }

  SUBCASE("user") {
    CHECK_OBJ_STR_EQ(o, "user", "dice");
  }

  SUBCASE("host") {
    CHECK_OBJ_STR_EQ(o, "host", "192.168.0.1");
  }

  SUBCASE("messageが存在しない") {
    CHECK(o.find("message") == o.end());
  }
}

TEST_CASE("Madoka JOIN with operator") {
  using irclog2json::message::MadokaLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLogLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("05:00:17 + dice2(~dice@sv1.trpg.net) to #もの書き with +o");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "JOIN");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 05:00:17 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "dice2");
  }

  SUBCASE("user") {
    CHECK_OBJ_STR_EQ(o, "user", "dice");
  }

  SUBCASE("host") {
    CHECK_OBJ_STR_EQ(o, "host", "sv1.trpg.net");
  }

  SUBCASE("messageが存在しない") {
    CHECK(o.find("message") == o.end());
  }
}
