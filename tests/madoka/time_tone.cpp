#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/madoka_line_converter.h"
#include "message/message_base.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka time tone") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2000-02-27", "%F", &tm_date);

  MadokaLineParser parser{"kataribe", tm_date};
  const auto m = parser.ToMessage("2000/02/07 03:00:00");

  REQUIRE_FALSE(m);
}

TEST_CASE("Madoka time tone at EOF") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2000-02-07", "%F", &tm_date);

  MadokaLineParser parser{"kataribe", tm_date};
  const auto m = parser.ToMessage("2000/02/08 00:00:00 end");

  REQUIRE_FALSE(m);
}
