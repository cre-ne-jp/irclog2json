#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"

#include "message/madoka_iso_2022_jp_line_converter.h"
#include "message/madoka_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP TOPIC") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("1999-02-21", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /* "00:38:04 Topic of channel #kataribe by sf: 創作TRPG語り部関係雑談"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3030 3a33 383a 3034 2054 6f70 6963 206f  00:38:04 Topic o
   * 00000010: 6620 6368 616e 6e65 6c20 236b 6174 6172  f channel #katar
   * 00000020: 6962 6520 6279 2073 663a 201b 2442 414f  ibe by sf: .$BAO
   * 00000030: 3a6e 1b28 4254 5250 471b 2442 386c 246a  :n.(BTRPG.$B8l$j
   * 00000040: 4974 3458 3738 3b28 434c 1b28 420a       It4X78;(CL.(B.
   */
  const auto m =
      parser.ToMessage("00:38:04 Topic of channel #kataribe by sf: "
                       "\x1B\x24\x42\x41\x4F\x3A\x6E\x1B\x28\x42"
                       "TRPG"
                       "\x1B\x24\x42\x38\x6C\x24\x6A\x49\x74\x34\x58\x37\x38"
                       "\x3B\x28\x43\x4C\x1B\x28\x42");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "TOPIC");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "kataribe");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "1999-02-21 00:38:04 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "sf");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "創作TRPG語り部関係雑談");
  }

  SUBCASE("iso2022JpCharsetFixed") {
    CHECK_UNARY_FALSE(o.at("iso2022JpCharsetFixed").get<bool>());
  }
}

TEST_CASE("Madoka ISO-2022-JP TOPIC 最後にASCII選択なし") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("1999-02-21", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  /* "00:38:04 Topic of channel #kataribe by sf: 創作TRPG語り部関係雑談"
   * xxdの出力（最後は LF = 0x0A）:
   * 00000000: 3030 3a33 383a 3034 2054 6f70 6963 206f  00:38:04 Topic o
   * 00000010: 6620 6368 616e 6e65 6c20 236b 6174 6172  f channel #katar
   * 00000020: 6962 6520 6279 2073 663a 201b 2442 414f  ibe by sf: .$BAO
   * 00000030: 3a6e 1b28 4254 5250 471b 2442 386c 246a  :n.(BTRPG.$B8l$j
   * 00000040: 4974 3458 3738 3b28 434c 1b28 420a       It4X78;(CL.(B.
   */
  const auto m = parser.ToMessage(
      "00:38:04 Topic of channel #kataribe by sf: "
      "\x1B\x24\x42\x41\x4F\x3A\x6E\x1B\x28\x42"
      "TRPG"
      "\x1B\x24\x42\x38\x6C\x24\x6A\x49\x74\x34\x58\x37\x38\x3B\x28\x43\x4C");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  SUBCASE("type") {
    CHECK_OBJ_STR_EQ(o, "type", "TOPIC");
  }

  SUBCASE("channel") {
    CHECK_OBJ_STR_EQ(o, "channel", "kataribe");
  }

  SUBCASE("timestamp") {
    CHECK_OBJ_STR_EQ(o, "timestamp", "1999-02-21 00:38:04 +0900");
  }

  SUBCASE("nick") {
    CHECK_OBJ_STR_EQ(o, "nick", "sf");
  }

  SUBCASE("message") {
    CHECK_OBJ_STR_EQ(o, "message", "創作TRPG語り部関係雑談");
  }

  SUBCASE("iso2022JpCharsetFixed") {
    CHECK_UNARY_FALSE(o.at("iso2022JpCharsetFixed").get<bool>());
  }
}

TEST_CASE("Madoka ISO-2022-JP TOPIC containing mIRC codes") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("1999-02-21", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  const auto m = parser.ToMessage(
      "00:38:04 Topic of channel #kataribe by sf: "
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0B\x0C\x0E\x0F"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1C\x1D\x1E\x1F"
      "visible characters");

  REQUIRE(m);

  const auto o = m->ToJsonObject();

  CHECK_OBJ_STR_EQ(o, "message",
                   "\x02\x03\x04\x0F\x11\x16\x1D\x1E\x1Fvisible characters");
}
