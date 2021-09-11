#include <regex>

#include "line_parser_re.h"

#include "tiarra_re.h"

#define TIARRA_CHANNEL_PATTERN CHANNEL_PATTERN R"(@[^\s]+)"

#define PRIVMSG_PATTERN                                                        \
  R"((\d{2}):(\d{2}):(\d{2}) (?:>(?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN   \
  ")<|<(?:" CHANNEL_PATTERN ":)?(" TARGET_PATTERN ")>) (.+)"

#define NOTICE_PATTERN                                                         \
  R"((\d{2}):(\d{2}):(\d{2}) (?:\)(?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN  \
  R"()\(|\((?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN R"()\)) (.+))"

#define NICK_MESSAGE_PATTERN_1                                                 \
  R"((\d{2}):(\d{2}):(\d{2}) ()" NICK_PATTERN ") -> (" NICK_PATTERN ")"

#define NICK_MESSAGE_PATTERN_2                                                 \
  R"((\d{2}):(\d{2}):(\d{2}) My nick is changed \(()" NICK_PATTERN             \
  ") [-=]> (" NICK_PATTERN R"()\))"

#define JOIN_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) \+ [+@]?()" NICK_PATTERN R"() \()" NICK_PATTERN   \
  "!~?(" USER_PATTERN ")@(" HOST_PATTERN R"()\) to )" TIARRA_CHANNEL_PATTERN

#define PART_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) - ()" NICK_PATTERN                                \
  ") from " TIARRA_CHANNEL_PATTERN R"((?: \((.*)\))?)"

#define QUIT_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) ! ()" NICK_PATTERN R"() \((.*)\))"

#define KICK_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) - ()" NICK_PATTERN ") by (" TARGET_PATTERN        \
  ") from " TIARRA_CHANNEL_PATTERN R"((?: \((.*)\))?)"

#define TOPIC_PATTERN                                                          \
  R"((\d{2}):(\d{2}):(\d{2}) Topic of channel )" TIARRA_CHANNEL_PATTERN        \
  " by (" TARGET_PATTERN "): (.*)"

namespace irclog2json {
namespace message {
namespace tiarra {

const std::regex RePrivmsg{PRIVMSG_PATTERN};
const std::regex ReNotice{NOTICE_PATTERN};
const std::regex ReNick1{NICK_MESSAGE_PATTERN_1};
const std::regex ReNick2{NICK_MESSAGE_PATTERN_2};
const std::regex ReJoin{JOIN_PATTERN};
const std::regex RePart{PART_PATTERN};
const std::regex ReQuit{QUIT_PATTERN};
const std::regex ReKick{KICK_PATTERN};
const std::regex ReTopic{TOPIC_PATTERN};

} // namespace tiarra
} // namespace message
} // namespace irclog2json
