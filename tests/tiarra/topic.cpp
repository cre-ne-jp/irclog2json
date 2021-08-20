#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/tiarra_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Tiarra NOTICE") {
  using irclog2json::message::TiarraLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  TiarraLogLineConverter converter{"もの書き予備", tm_date};
  const auto m = converter.ToMessage("09:46:31 Topic of channel #もの書き予備@cre by irc.sougetu.net: 参加歓迎【ログ公開】文章を書くこと読むこと全般(予備その1)");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "TOPIC");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "もの書き予備");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 09:46:31 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "irc.sougetu.net");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "参加歓迎【ログ公開】文章を書くこと読むこと全般(予備その1)");
  }
}