#include "basic_message.h"
#include "kick.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    Kick::Kick(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& target, std::string const& message) :
      BasicMessage::BasicMessage("KICK", channel, timestamp, nick, message),
      target_(target)
    {
    }

    picojson::object Kick::DoToJsonObject() const {
      picojson::object m = BasicMessage::DoToJsonObject();

      m["target"] = picojson::value(target_);

      return m;
    }
  }
}
