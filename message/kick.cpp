#include "message_base.h"
#include "kick.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    Kick::Kick(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& target, std::string const& message) :
      MessageBase::MessageBase("KICK", channel, timestamp, nick),
      target_(target),
      message_(message)
    {
    }

    picojson::object Kick::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["target"] = picojson::value(target_);
      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
