#include <regex>

#include "line_parser_re.h"

#include "madoka_re.h"

#define PRIVMSG_PATTERN                                                        \
  R"((\d{2}):(\d{2}):(\d{2}) (?:>(?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN   \
  ")<|<(?:" CHANNEL_PATTERN ":)?(" TARGET_PATTERN ")>) (.+)"

#define NOTICE_PATTERN                                                         \
  R"((\d{2}):(\d{2}):(\d{2}) (?:\)(?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN  \
  R"()\(|\((?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN R"()\)) (.+))"

#define NICK_MESSAGE_PATTERN_1                                                 \
  R"((\d{2}):(\d{2}):(\d{2}) ()" NICK_PATTERN ") -> (" NICK_PATTERN ")"

#define NICK_MESSAGE_PATTERN_2                                                 \
  R"((\d{2}):(\d{2}):(\d{2}) \[!\] nick changed \(()" NICK_PATTERN             \
  ") -> (" NICK_PATTERN R"()\))"

// 01:07:41 + licorice_(~licorice@home.cokage.ne.jp) to #もの書き
#define JOIN_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) \+ [+@]?()" NICK_PATTERN                          \
  R"()\([-+~]?()" USER_PATTERN ")@(" HOST_PATTERN R"()\) to )" CHANNEL_PATTERN \
  R"((?: with \+[a-z]+)?)"

#define PART_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) - ()" NICK_PATTERN ") from " CHANNEL_PATTERN      \
  R"((?: \((.*)\))?)"

#define QUIT_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) ! ()" NICK_PATTERN R"raw()(?: \((.*)\))?)raw"

#define KICK_PATTERN                                                           \
  R"((\d{2}):(\d{2}):(\d{2}) - ()" NICK_PATTERN ") by (" TARGET_PATTERN        \
  ") from " CHANNEL_PATTERN R"((?: \((.*)\))?)"

#define TOPIC_PATTERN                                                          \
  R"((\d{2}):(\d{2}):(\d{2}) Topic of channel )" CHANNEL_PATTERN               \
  " by (" TARGET_PATTERN "): (.*)"

#define AUTO_DOWN_PATTERN R"((\d{2}):(\d{2}):(\d{2}) \[!\] auto down)"

namespace irclog2json {
namespace message {
namespace madoka {

const std::regex RePrivmsg{PRIVMSG_PATTERN};
const std::regex ReNotice{NOTICE_PATTERN};
const std::regex ReNick1{NICK_MESSAGE_PATTERN_1};
const std::regex ReNick2{NICK_MESSAGE_PATTERN_2};
const std::regex ReJoin{JOIN_PATTERN};
const std::regex RePart{PART_PATTERN};
const std::regex ReQuit{QUIT_PATTERN};
const std::regex ReKick{KICK_PATTERN};
const std::regex ReTopic{TOPIC_PATTERN};
const std::regex ReAutoDown{AUTO_DOWN_PATTERN};

} // namespace madoka
} // namespace message
} // namespace irclog2json
