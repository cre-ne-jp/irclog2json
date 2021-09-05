#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"

#include "message/madoka_iso_2022_jp_line_converter.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP QUIT with message") {
  using irclog2json::message::MadokaIso2022JpLineConverter;
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto converter_madoka =
      std::make_unique<MadokaLineConverter>(channel, tm_date);
  MadokaIso2022JpLineConverter converter{std::move(converter_madoka)};

  /*
   * "19:04:12 ! ocha (\"さようなら\")"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3139 3a30 343a 3132 2021 206f 6368 6120  19:04:12 ! ocha
   * 00000010: 2822 1b24 4224 3524 6824 2624 4a24 691b  (".$B$5$h$&$J$i.
   * 00000020: 2842 2229 0a                             (B").
   */
  const auto m = converter.ToMessage(
      R"(19:04:12 ! ocha (")"
      "\x1B\x24\x42\x24\x35\x24\x68\x24\x26\x24\x4A\x24\x69\x1B\x28\x42"
      R"raw("))raw");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "QUIT");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
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

TEST_CASE("Madoka ISO-2022-JP QUIT with message 最後にASCII選択なし") {
  using irclog2json::message::MadokaIso2022JpLineConverter;
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto converter_madoka =
      std::make_unique<MadokaLineConverter>(channel, tm_date);
  MadokaIso2022JpLineConverter converter{std::move(converter_madoka)};

  /*
   * "19:04:12 ! ocha (\"さようなら\")"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3139 3a30 343a 3132 2021 206f 6368 6120  19:04:12 ! ocha
   * 00000010: 2822 1b24 4224 3524 6824 2624 4a24 691b  (".$B$5$h$&$J$i.
   * 00000020: 2842 2229 0a                             (B").
   */
  const auto m =
      converter.ToMessage(R"(19:04:12 ! ocha (")"
                          "\x1B\x24\x42\x24\x35\x24\x68\x24\x26\x24\x4A\x24\x69"
                          R"raw("))raw");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "QUIT");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
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
