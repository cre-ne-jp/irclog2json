#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"

#include "message/madoka_iso_2022_jp_line_parser.h"
#include "message/madoka_line_parser.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP KICK with message") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /*
   * "08:54:52 - ocha by Toybox from #cre (ごめんね。)"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3038 3a35 343a 3532 202d 206f 6368 6120  08:54:52 - ocha
   * 00000010: 6279 2054 6f79 626f 7820 6672 6f6d 2023  by Toybox from #
   * 00000020: 6372 6520 281b 2442 2434 2461 2473 244d  cre (.$B$4$a$s$M
   * 00000030: 2123 1b28 4229 0a                        !#.(B).
   */
  const auto m = parser.ToMessage(
      "08:54:52 - ocha by Toybox from #cre ("
      "\x1B\x24\x42\x24\x34\x24\x61\x24\x73\x24\x4D\x21\x23\x1B\x28\x42"
      ")");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "KICK");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
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

  SUBCASE("iso2022JpCharsetFixed") {
    CHECK_UNARY_FALSE(o.at("iso2022JpCharsetFixed").get<bool>());
  }
}

TEST_CASE("Madoka ISO-2022-JP KICK with message 最後にASCII選択なし") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /*
   * "08:54:52 - ocha by Toybox from #cre (ごめんね。)"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3038 3a35 343a 3532 202d 206f 6368 6120  08:54:52 - ocha
   * 00000010: 6279 2054 6f79 626f 7820 6672 6f6d 2023  by Toybox from #
   * 00000020: 6372 6520 281b 2442 2434 2461 2473 244d  cre (.$B$4$a$s$M
   * 00000030: 2123 1b28 4229 0a                        !#.(B).
   */
  const auto m =
      parser.ToMessage("08:54:52 - ocha by Toybox from #cre ("
                       "\x1B\x24\x42\x24\x34\x24\x61\x24\x73\x24\x4D\x21\x23"
                       ")");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "KICK");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
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

  SUBCASE("iso2022JpCharsetFixed") {
    CHECK_UNARY(o.at("iso2022JpCharsetFixed").get<bool>());
  }
}

TEST_CASE("Madoka ISO-2022-JP KICK with message containing mIRC codes") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  const auto m = parser.ToMessage(
      "08:54:52 - ocha by Toybox from #cre ("
      "abc"
      "\x1B\x24\x42\x24\x46\x24\x39\x24\x48\x1B\x28\x42"
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0B\x0C\x0E\x0F"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1C\x1D\x1E\x1F"
      "visible characters"
      ")");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  CHECK_OBJ_STR_EQ(
      o, "message",
      "abcてすと\x02\x03\x04\x0F\x11\x16\x1D\x1E\x1Fvisible characters");
}
