#include <ctime>
#include <memory>
#include <regex>
#include <string>

#include "line_converter_re.h"
#include "utf8_line_converter.h"

#include "madoka_line_converter.h"
#include "madoka_re.h"

#include "message_base.h"

#include "basic_message.h"
#include "join.h"
#include "kick.h"
#include "madoka_auto_down.h"

namespace irclog2json {
namespace message {

MadokaLineParser::~MadokaLineParser() = default;

std::unique_ptr<MessageBase>
MadokaLineParser::DoToMessage(std::string const& line) const {
  std::smatch m;
  struct tm timestamp = {};

  if (std::regex_match(line, m, madoka::RePrivmsg)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4].length() > 0 ? m[4] : m[5]);
    std::string message(RemoveControlCodes(m[6].str()));

    return std::make_unique<BasicMessage>("PRIVMSG", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, madoka::ReNotice)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4].length() > 0 ? m[4] : m[5]);
    std::string message(RemoveControlCodes(m[6].str()));

    return std::make_unique<BasicMessage>("NOTICE", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, madoka::ReNick1) ||
      std::regex_match(line, m, madoka::ReNick2)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4]);
    std::string new_nick(m[5]);

    return std::make_unique<BasicMessage>("NICK", channel_, timestamp, nick,
                                          new_nick);
  }

  if (std::regex_match(line, m, madoka::ReJoin)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4]);
    std::string user(m[5]);
    std::string host(m[6]);

    return std::make_unique<Join>(channel_, timestamp, nick, user, host);
  }

  if (std::regex_match(line, m, madoka::RePart)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4]);
    std::string message(RemoveControlCodes(m[5].str()));

    return std::make_unique<BasicMessage>("PART", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, madoka::ReQuit)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4]);
    std::string message(
        RemoveControlCodes(m[5].length() > 0 ? m[5].str() : ""));

    return std::make_unique<BasicMessage>("QUIT", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, madoka::ReKick)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[5]);
    std::string target(m[4]);
    std::string message(RemoveControlCodes(m[6].str()));

    return std::make_unique<Kick>(channel_, timestamp, nick, target, message);
  }

  if (std::regex_match(line, m, madoka::ReTopic)) {
    timestamp = GetTimestamp(m);

    std::string nick(m[4]);
    std::string message(RemoveControlCodes(m[5].str()));

    return std::make_unique<BasicMessage>("TOPIC", channel_, timestamp, nick,
                                          message);
  }

  if (std::regex_match(line, m, madoka::ReAutoDown)) {
    timestamp = GetTimestamp(m);
    return std::make_unique<MadokaAutoDown>(channel_, timestamp);
  }

  return std::unique_ptr<MessageBase>{};
}

} // namespace message
} // namespace irclog2json
