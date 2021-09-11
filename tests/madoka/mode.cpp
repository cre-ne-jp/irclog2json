#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/madoka_line_parser.h"
#include "message/message_base.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka MODE") {
  using irclog2json::message::MadokaLineParser;

  struct tm tm_date {};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLineParser parser{"もの書き", tm_date};
  const auto m =
      parser.ToMessage("01:05:52 Mode by Toybox: #もの書き +v ocha ");

  REQUIRE_FALSE(m);
}
