#include "factory.h"

#include "message_base.h"
#include "privmsg.h"
#include "notice.h"
#include "nick.h"
#include "join.h"
#include "part.h"
#include "quit.h"
#include "kick.h"
#include "topic.h"

#include <string>
#include <regex>
#include <memory>
#include <ctime>

#define RE_LETTER "[A-Za-z]"
#define RE_SPECIAL R"([\x5B-\x60\x7B-\x7D])"
// RFC 2812 より寛容
#define NICK_PATTERN "(?:" RE_LETTER R"(|\d|)" RE_SPECIAL "|-)+"
#define USER_PATTERN R"([^\x00\x0A\x0D\x20\x40]+)"
#define RE_CHANSTRING R"([^\x00\x07\x0A\x0D\x20,:]+)"
#define CHANNEL_PATTERN "[#+&]" RE_CHANSTRING
#define TIARRA_CHANNEL_PATTERN CHANNEL_PATTERN R"(@\w+)"

#define RE_IP4ADDR R"((?:\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))"
// ホスト名はとりあえず IPv4 のみ
#define RE_HOSTADDR RE_IP4ADDR
#define RE_SHORTNAME "(?:" RE_LETTER R"(|\d)(?:)" RE_LETTER R"(|\d|-)*(?:)" RE_LETTER R"(|\d)*)"
#define RE_HOSTNAME RE_SHORTNAME R"((?:\.)" RE_SHORTNAME R"()*\.?)"
#define HOST_PATTERN "(?:" RE_HOSTNAME "|" RE_HOSTADDR ")"
#define TARGET_PATTERN "(?:" NICK_PATTERN "|" HOST_PATTERN ")"

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

  std::string RemoveControlCodes(std::string const& s) {
    static const std::regex re(R"([\x02\x03\x1D\x1F\x16\x0F])");
    return std::regex_replace(s, re, "");
  }
}

namespace irclog2json {
  namespace message {

    Factory::Factory(std::string const& channel, struct tm const& tm_date) :
      channel_(channel),
      tm_date_(tm_date)
    {
    }

    std::unique_ptr<MessageBase> Factory::ToMessage(std::string const& line) const {
      std::smatch m;
      struct tm timestamp;

      if (regex_match(line, m, RePrivmsg)) {
        SetHMS(timestamp, m);

        std::string nick(m[4].length() > 0 ? m[4] : m[5]);
        std::string message(RemoveControlCodes(m[6].str()));

        return std::make_unique<Privmsg>(channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReNotice)) {
        SetHMS(timestamp, m);

        std::string nick(m[4].length() > 0 ? m[4] : m[5]);
        std::string message(RemoveControlCodes(m[6].str()));

        return std::make_unique<Notice>(channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReNick1) || regex_match(line, m, ReNick2)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string new_nick(m[5]);

        return std::make_unique<Nick>(channel_, timestamp, nick, new_nick);
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

        return std::make_unique<Part>(channel_, timestamp, nick, message);
      }

      if (regex_match(line, m, ReQuit)) {
        SetHMS(timestamp, m);

        std::string nick(m[4]);
        std::string message(RemoveControlCodes(m[5].str()));

        return std::make_unique<Quit>(channel_, timestamp, nick, message);
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

        return std::make_unique<Topic>(channel_, timestamp, nick, message);
      }

      return std::unique_ptr<MessageBase>(nullptr);
    }

    void Factory::SetHMS(struct tm& timestamp, std::smatch const& m) const {
      timestamp = tm_date_;

      timestamp.tm_hour = std::stoi(m[1]);
      timestamp.tm_min = std::stoi(m[2]);
      timestamp.tm_sec = std::stoi(m[3]);
    }
  }
}
