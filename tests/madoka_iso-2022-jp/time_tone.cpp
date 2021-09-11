/**
 * @file time_tone.cpp
 * @brief ISO-2022-JPのMadoka形式ログの時報のテスト。SEGV検出のために追加した。
 */

#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>
#include <string>

#include <picojson.h>

#include "message/message_base.h"

#include "message/madoka_iso_2022_jp_line_parser.h"
#include "message/madoka_line_parser.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP time tone") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2000-02-27", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  const auto m = parser.ToMessage("2000/02/07 03:00:00");

  REQUIRE_FALSE(m);
}

TEST_CASE("Madoka ISO-2022-JP time tone at EOF") {
  using irclog2json::message::MadokaIso2022JpLineParser;
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2000-02-07", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto parser_madoka = std::make_unique<MadokaLineParser>(channel, tm_date);
  MadokaIso2022JpLineParser parser{std::move(parser_madoka)};

  const auto m = parser.ToMessage("2000/02/08 00:00:00 end");

  REQUIRE_FALSE(m);
}
