#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <ctime>
#include <memory>
#include <sstream>
#include <string>

#include <picojson.h>

#include "madoka_log_parser.h"

#include "message/madoka_auto_down.h"
#include "message/madoka_line_parser.h"
#include "message/message_base.h"

#include "tests/test_helper.h"

TEST_CASE("Madoka: auto downがない場合、解析に成功する") {
  using irclog2json::MadokaLogParser;
  using irclog2json::message::MadokaAutoDown;
  using irclog2json::message::MadokaLineParser;
  using irclog2json::message::MessageBase;

  struct tm tm_date {};
  strptime("2021-04-01", "%F", &tm_date);

  std::string log{"10:55:30 <#もの書き:foo> てすと\n"
                  "10:56:52 + bar(~bar@example.net) to #もの書き\n"
                  "11:00:16 + cre_log(~log@example.net) to #もの書き\n"
                  "11:23:34 + baz(~baz@example.net) to #もの書き\n"
                  "12:55:30 <#もの書き:foo> てすと\n"
                  "12:58:31 [!] nick changed (cre_log -> cre_log2)\n"};
  std::istringstream log_ss{log};

  auto line_parser = std::make_unique<MadokaLineParser>("もの書き", tm_date);
  MadokaLogParser parser{&log_ss, std::move(line_parser)};

  auto messages = parser.ExtractMessages();

  CHECK_EQ(messages.size(), 6);
}

TEST_CASE("Madoka auto down: ニックネームが正しく設定される") {
  using irclog2json::MadokaLogParser;
  using irclog2json::message::MadokaAutoDown;
  using irclog2json::message::MadokaLineParser;
  using irclog2json::message::MessageBase;

  struct tm tm_date {};
  strptime("2021-04-01", "%F", &tm_date);

  std::string log{"10:55:30 <#もの書き:foo> てすと\n"
                  "10:56:52 + bar(~bar@example.net) to #もの書き\n"
                  "11:00:00 [!] auto down\n"
                  "11:00:16 + cre_log(~log@example.net) to #もの書き\n"
                  "11:23:34 + baz(~baz@example.net) to #もの書き\n"
                  "12:55:30 <#もの書き:foo> てすと\n"
                  "12:56:52 + bar(~bar@example.net) to #もの書き\n"
                  "12:58:31 [!] nick changed (cre_log -> cre_log2)\n"
                  "13:00:00 [!] auto down\n"
                  "13:00:16 + cre_log2(~log@example.net) to #もの書き\n"
                  "13:23:34 + cre(~cre@example.net) to #もの書き\n"};
  std::istringstream log_ss{log};

  auto line_parser = std::make_unique<MadokaLineParser>("もの書き", tm_date);
  MadokaLogParser parser{&log_ss, std::move(line_parser)};

  auto messages = parser.ExtractMessages();

  REQUIRE_EQ(messages.size(), 11);

  MadokaAutoDown* auto_down = dynamic_cast<MadokaAutoDown*>(messages[2].get());
  REQUIRE_MESSAGE(auto_down, "messages[2] はMadokaAutoDown");

  std::string auto_down_nick = auto_down->GetNick();
  CHECK_EQ(auto_down_nick, "cre_log");

  MadokaAutoDown* auto_down_2 =
      dynamic_cast<MadokaAutoDown*>(messages[8].get());
  REQUIRE_MESSAGE(auto_down, "messages[8] はMadokaAutoDown");

  std::string auto_down_2_nick = auto_down_2->GetNick();
  CHECK_EQ(auto_down_2_nick, "cre_log2");
}
