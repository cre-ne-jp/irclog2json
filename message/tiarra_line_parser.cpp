#include <ctime>
#include <memory>
#include <regex>
#include <string>

#include "line_parser_re.h"
#include "utf8_line_parser.h"

#include "tiarra_line_parser.h"
#include "tiarra_re.h"

#include "message_base.h"

#include "basic_message.h"
#include "join.h"
#include "kick.h"

namespace irclog2json {
namespace message {

TiarraLineParser::~TiarraLineParser() = default;

std::unique_ptr<MessageBase>
TiarraLineParser::DoToMessage(const std::string& line) const {
  std::smatch m;
  struct tm timestamp = {};

  if (std::regex_match(line, m, tiarra::RePrivmsg)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4].length() > 0 ? m[4] : m[5]};
    std::string message{RemoveControlCodes(m[6].str())};

    return std::make_unique<BasicMessage>("PRIVMSG", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, tiarra::ReNotice)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4].length() > 0 ? m[4] : m[5]};
    std::string message{RemoveControlCodes(m[6].str())};

    return std::make_unique<BasicMessage>("NOTICE", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, tiarra::ReNick1) ||
      std::regex_match(line, m, tiarra::ReNick2)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4]};
    std::string new_nick{m[5]};

    return std::make_unique<BasicMessage>("NICK", channel_, timestamp, nick,
                                          new_nick);
  }

  if (std::regex_match(line, m, tiarra::ReJoin)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4]};
    std::string user{m[5]};
    std::string host{m[6]};

    return std::make_unique<Join>(channel_, timestamp, nick, user, host);
  }

  if (std::regex_match(line, m, tiarra::RePart)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4]};
    std::string message{RemoveControlCodes(m[5].str())};

    return std::make_unique<BasicMessage>("PART", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, tiarra::ReQuit)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4]};
    std::string message{RemoveControlCodes(m[5].str())};

    return std::make_unique<BasicMessage>("QUIT", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, tiarra::ReKick)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[5]};
    std::string target{m[4]};
    std::string message{RemoveControlCodes(m[6].str())};

    return std::make_unique<Kick>(channel_, timestamp, nick, target, message);
  }

  if (std::regex_match(line, m, tiarra::ReTopic)) {
    timestamp = GetTimestamp(m);

    std::string nick{m[4]};
    std::string message{RemoveControlCodes(m[5].str())};

    return std::make_unique<BasicMessage>("TOPIC", channel_, timestamp, nick,
                                          message);
  }

  return std::unique_ptr<MessageBase>{};
}

} // namespace message
} // namespace irclog2json
