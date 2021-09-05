#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>

#include <picojson.h>

#include "message/message_base.h"
#include "message/tiarra_log_line_converter.h"

#include "tests/test_helper.h"

TEST_CASE("Tiarra MODE") {
  using irclog2json::message::TiarraLineConverter;

  struct tm tm_date{};

  strptime("2021-04-01", "%F", &tm_date);

  TiarraLineConverter converter{"もの書き予備", tm_date};
  const auto m = converter.ToMessage("09:46:31 Mode by irc.sougetu.net: #もの書き予備@cre -o Toybox");

  REQUIRE_FALSE(m);
}
