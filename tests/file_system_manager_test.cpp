#define _XOPEN_SOURCE

#include <doctest/doctest.h>

#include <string>

#include <ctime>

#include "file_name_manager.h"

TEST_CASE("1999-02-22") {
  const char* input_path = "/home/irc/log/19990222.txt";

  irclog2json::FileNameManager file_name_manager{std::string{input_path}};

  SUBCASE("GetDate") {
    std::optional<struct tm> tm_date = file_name_manager.GetDate();

    REQUIRE_UNARY(tm_date);

    CHECK_EQ(tm_date->tm_year, 1999 - 1900);
    CHECK_EQ(tm_date->tm_mon, 2 - 1);
    CHECK_EQ(tm_date->tm_mday, 22);
  }

  SUBCASE("GetOutputPath") {
    CHECK_EQ(file_name_manager.GetOutputPath(), "/home/irc/log/19990222.json");
  }
}

TEST_CASE("2016-11-27") {
  const char* input_path = "/home/irc/log/20161127.txt";

  irclog2json::FileNameManager file_name_manager{std::string{input_path}};

  SUBCASE("GetDate") {
    std::optional<struct tm> tm_date = file_name_manager.GetDate();

    REQUIRE_UNARY(tm_date);

    CHECK_EQ(tm_date->tm_year, 2016 - 1900);
    CHECK_EQ(tm_date->tm_mon, 11 - 1);
    CHECK_EQ(tm_date->tm_mday, 27);
  }

  SUBCASE("GetOutputPath") {
    CHECK_EQ(file_name_manager.GetOutputPath(), "/home/irc/log/20161127.json");
  }
}

TEST_CASE("Invalid date: GetDate") {
  const char* input_path = "/home/irc/log/20210401example.txt";

  irclog2json::FileNameManager file_name_manager{std::string{input_path}};

  std::optional<struct tm> tm_date = file_name_manager.GetDate();

  REQUIRE_UNARY_FALSE(tm_date);
}
