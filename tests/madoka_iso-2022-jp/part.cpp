#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"
#include "message/iso_2022_jp_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP PART with message") {
  using irclog2json::message::MadokaLogLineConverter;
  using irclog2json::message::Iso2022JpLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  MadokaLogLineConverter converter_madoka{channel, tm_date};
  Iso2022JpLineConverter converter{channel, tm_date, converter_madoka};

  /*
   * "19:04:12 - ocha from #cre (さようなら)"
   * xxdの出力（最後は LF = 0x0A）
   * 00000000: 3139 3a30 343a 3132 202d 206f 6368 6120  19:04:12 - ocha
   * 00000010: 6672 6f6d 2023 6372 6520 281b 2442 2435  from #cre (.$B$5
   * 00000020: 2468 2426 244a 2469 1b28 4229 0a         $h$&$J$i.(B).
   */
  const auto m = converter.ToMessage(
    "19:04:12 - ocha from #cre ("
    "\x1B\x24\x42\x24\x35\x24\x68\x24\x26\x24\x4A\x24\x69\x1B\x28\x42"
    ")"
  );

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "PART");
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
    CHECK_OBJ_STR_EQ(o, "message", "さようなら");
  }
}

TEST_CASE("Madoka ISO-2022-JP PART with message 最後にASCII選択なし") {
  using irclog2json::message::MadokaLogLineConverter;
  using irclog2json::message::Iso2022JpLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  MadokaLogLineConverter converter_madoka{channel, tm_date};
  Iso2022JpLineConverter converter{channel, tm_date, converter_madoka};

  /*
   * "19:04:12 - ocha from #cre (さようなら)"
   * xxdの出力（最後は LF = 0x0A）
   * 00000000: 3139 3a30 343a 3132 202d 206f 6368 6120  19:04:12 - ocha
   * 00000010: 6672 6f6d 2023 6372 6520 281b 2442 2435  from #cre (.$B$5
   * 00000020: 2468 2426 244a 2469 1b28 4229 0a         $h$&$J$i.(B).
   */
  const auto m = converter.ToMessage(
    "19:04:12 - ocha from #cre ("
    "\x1B\x24\x42\x24\x35\x24\x68\x24\x26\x24\x4A\x24\x69"
    ")"
  );

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "PART");
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
    CHECK_OBJ_STR_EQ(o, "message", "さようなら");
  }
}
