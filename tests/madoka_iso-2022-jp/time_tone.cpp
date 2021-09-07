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

#include "message/madoka_iso_2022_jp_line_converter.h"
#include "message/madoka_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka ISO-2022-JP time tone") {
  using irclog2json::message::MadokaIso2022JpLineConverter;
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date {};

  strptime("2000-02-27", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto converter_madoka =
      std::make_unique<MadokaLineConverter>(channel, tm_date);
  MadokaIso2022JpLineConverter converter{std::move(converter_madoka)};

  const auto m = converter.ToMessage("2000/02/07 03:00:00");

  REQUIRE_FALSE(m);
}

TEST_CASE("Madoka ISO-2022-JP time tone at EOF") {
  using irclog2json::message::MadokaIso2022JpLineConverter;
  using irclog2json::message::MadokaLineConverter;

  struct tm tm_date {};

  strptime("2000-02-07", "%F", &tm_date);

  const std::string channel{"kataribe"};
  auto converter_madoka =
      std::make_unique<MadokaLineConverter>(channel, tm_date);
  MadokaIso2022JpLineConverter converter{std::move(converter_madoka)};

  const auto m = converter.ToMessage("2000/02/08 00:00:00 end");

  REQUIRE_FALSE(m);
}
