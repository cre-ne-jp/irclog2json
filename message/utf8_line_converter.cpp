#include <ctime>
#include <memory>
#include <regex>
#include <string>

#include "message_base.h"
#include "utf8_line_converter.h"

namespace irclog2json {
namespace message {

std::string UTF8LineConverter::RemoveControlCodes(std::string const& s) {
  static const std::regex re{R"([\x00\x01\x05-\x0E\x10\x12-\x15\x17-\x1C])"};
  return std::regex_replace(s, re, "");
}

UTF8LineConverter::UTF8LineConverter(std::string const& channel,
                                     struct tm const& tm_date)
    : channel_{channel}, tm_date_{tm_date} {
}

struct tm UTF8LineConverter::GetTimestamp(const std::smatch& m) const {
  struct tm timestamp {
    tm_date_
  };

  timestamp.tm_hour = std::stoi(m[1]);
  timestamp.tm_min = std::stoi(m[2]);
  timestamp.tm_sec = std::stoi(m[3]);

  return timestamp;
}

} // namespace message
} // namespace irclog2json
