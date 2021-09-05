#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka QUIT without message") {
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("07:23:28 ! ocha");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "QUIT");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 07:23:28 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "ocha");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "");
  }
}

TEST_CASE("Madoka QUIT with message") {
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage(R"raw(19:04:12 ! ocha ("さようなら"))raw");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "QUIT");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 19:04:12 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "ocha");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", R"("さようなら")");
  }
}
