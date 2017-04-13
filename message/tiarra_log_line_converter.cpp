#include "line_converter_base.h"
#include "line_converter_re.h"
#include "tiarra_log_line_converter.h"

#include "message_base.h"
#include "basic_message.h"
#include "join.h"
#include "kick.h"

#include <string>
#include <regex>
#include <memory>
#include <ctime>

#define TIARRA_CHANNEL_PATTERN CHANNEL_PATTERN R"(@[^\s]+)"

#define PRIVMSG_PATTERN R"((\d{2}):(\d{2}):(\d{2}) (?:>(?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN ")<|<(?:" CHANNEL_PATTERN ":)?(" TARGET_PATTERN ")>) (.+)"
#define NOTICE_PATTERN R"((\d{2}):(\d{2}):(\d{2}) (?:\)(?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN R"()\(|\((?:)" CHANNEL_PATTERN ":)?(" TARGET_PATTERN R"()\)) (.+))"
#define NICK_MESSAGE_PATTERN_1 R"((\d{2}):(\d{2}):(\d{2}) ()" NICK_PATTERN ") -> (" NICK_PATTERN ")"
#define NICK_MESSAGE_PATTERN_2 R"((\d{2}):(\d{2}):(\d{2}) My nick is changed \(()" NICK_PATTERN ") [-=]> (" NICK_PATTERN R"()\))"
#define JOIN_PATTERN R"((\d{2}):(\d{2}):(\d{2}) \+ [+@]?()" NICK_PATTERN R"() \()" NICK_PATTERN "!~?(" USER_PATTERN ")@(" HOST_PATTERN R"()\) to )" TIARRA_CHANNEL_PATTERN
#define PART_PATTERN R"((\d{2}):(\d{2}):(\d{2}) - ()" NICK_PATTERN ") from " TIARRA_CHANNEL_PATTERN R"((?: \((.*)\))?)"
#define QUIT_PATTERN R"((\d{2}):(\d{2}):(\d{2}) ! ()" NICK_PATTERN R"() \((.*)\))"
#define KICK_PATTERN R"((\d{2}):(\d{2}):(\d{2}) - ()" NICK_PATTERN ") by (" TARGET_PATTERN ") from " TIARRA_CHANNEL_PATTERN R"((?: \((.*)\))?)"
#define TOPIC_PATTERN R"((\d{2}):(\d{2}):(\d{2}) Topic of channel )" TIARRA_CHANNEL_PATTERN " by (" TARGET_PATTERN "): (.*)"

namespace {
  const std::regex RePrivmsg(PRIVMSG_PATTERN);
  const std::regex ReNotice(NOTICE_PATTERN);
  const std::regex ReNick1(NICK_MESSAGE_PATTERN_1);
  const std::regex ReNick2(NICK_MESSAGE_PATTERN_2);
  const std::regex ReJoin(JOIN_PATTERN);
  const std::regex RePart(PART_PATTERN);
  const std::regex ReQuit(QUIT_PATTERN);
  const std::regex ReKick(KICK_PATTERN);
  const std::regex ReTopic(TOPIC_PATTERN);
}

namespace irclog2json {
  namespace message {
    TiarraLogLineConverter::~TiarraLogLineConverter() = default;

    std::unique_ptr<MessageBase> TiarraLogLineConverter::DoToMessage(std::string const& line) const {
      std::smatch m;
      struct tm timestamp;

      if (regex_match(line, m, RePrivmsg)) {
        SetHMS(timestamp, m);

        std::string nick(m[4].length() > 0 ? m[4] : m[5]);
        std::string message(RemoveControlCodes(m[6].str()));

        return std::make_unique<BasicMessage>("PRIVMSG", channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReNotice)) {
        SetHMS(timestamp, m);

        std::string nick(m[4].length() > 0 ? m[4] : m[5]);
        std::string message(RemoveControlCodes(m[6].str()));

        return std::make_unique<BasicMessage>("NOTICE", channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReNick1) || regex_match(line, m, ReNick2)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string new_nick(m[5]);

        return std::make_unique<BasicMessage>("NICK", channel_, timestamp, nick, new_nick);
      }

      if (regex_match(line, m, ReJoin)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string user(m[5]);
        std::string host(m[6]);

        return std::make_unique<Join>(channel_, timestamp, nick, user, host);
      }

      if (regex_match(line, m, RePart)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string message(RemoveControlCodes(m[5].str()));

        return std::make_unique<BasicMessage>("PART", channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReQuit)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string message(RemoveControlCodes(m[5].str()));

        return std::make_unique<BasicMessage>("QUIT", channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReKick)) {
        SetHMS(timestamp, m);

        std::string nick(m[5]);
        std::string target(m[4]);
        std::string message(RemoveControlCodes(m[6].str()));

        return std::make_unique<Kick>(channel_, timestamp, nick, target, message);
      }

      if (regex_match(line, m, ReTopic)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string message(RemoveControlCodes(m[5].str()));

        return std::make_unique<BasicMessage>("TOPIC", channel_, timestamp, nick, message);
      }

      return std::unique_ptr<MessageBase>(nullptr);
    }
  }
}
