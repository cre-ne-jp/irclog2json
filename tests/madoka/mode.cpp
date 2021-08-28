#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/madoka_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka MODE") {
  using irclog2json::message::MadokaLogLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  MadokaLogLineConverter converter{"もの書き", tm_date};
  const auto m = converter.ToMessage("01:05:52 Mode by Toybox: #もの書き +v ocha ");

  REQUIRE_FALSE(m);
}
