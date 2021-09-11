#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"

#include "message/madoka_iso_2022_jp_line_converter.h"
#include "message/madoka_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP PRIVMSG") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /*
   * "12:34:56 <#cre:Toybox> てすと"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3132 3a33 343a 3536 203c 2363 7265 3a54  12:34:56 <#cre:T
   * 00000010: 6f79 626f 783e 201b 2442 2446 2439 2448  oybox> .$B$F$9$H
   * 00000020: 1b28 420a                                .(B.
   */
  const auto m =
      parser.ToMessage("12:34:56 <#cre:Toybox> "
                       "\x1B\x24\x42\x24\x46\x24\x39\x24\x48\x1B\x28\x42");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "PRIVMSG");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 12:34:56 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "てすと");
  }

  SUBCASE("iso2022JpCharsetFixed") {
    CHECK_UNARY_FALSE(o.at("iso2022JpCharsetFixed").get<bool>());
  }
}

TEST_CASE("Madoka ISO-2022-JP PRIVMSG 最後にASCII選択なし") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  std::string channel{"cre"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /*
   * "12:34:56 <#cre:Toybox> てすと"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3132 3a33 343a 3536 203c 2363 7265 3a54  12:34:56 <#cre:T
   * 00000010: 6f79 626f 783e 201b 2442 2446 2439 2448  oybox> .$B$F$9$H
   * 00000020: 1b28 420a                                .(B.
   */
  const auto m = parser.ToMessage(
      "12:34:56 <#cre:Toybox> \x1B\x24\x42\x24\x46\x24\x39\x24\x48");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "PRIVMSG");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "cre");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "2021-04-01 12:34:56 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "Toybox");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "てすと");
  }

  SUBCASE("iso2022JpCharsetFixed") {
    CHECK_UNARY_FALSE(o.at("iso2022JpCharsetFixed").get<bool>());
  }
}

TEST_CASE("Madoka ISO-2022-JP PRIVMSG containing mIRC codes") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  const std::string channel{"cre"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /*
   * "12:34:56 <#cre:Toybox> てすと"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3132 3a33 343a 3536 203c 2363 7265 3a54  12:34:56 <#cre:T
   * 00000010: 6f79 626f 783e 201b 2442 2446 2439 2448  oybox> .$B$F$9$H
   * 00000020: 1b28 420a                                .(B.
   */
  const auto m = parser.ToMessage(
      "12:34:56 <#cre:Toybox> "
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0B\x0C\x0E\x0F"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1C\x1D\x1E\x1F"
      "visible characters");
  REQUIRE(m);

  const auto o = m->ToJsonObject();

  CHECK_OBJ_STR_EQ(o, "message",
                   "\x02\x03\x04\x0F\x11\x16\x1D\x1E\x1Fvisible characters");
}
