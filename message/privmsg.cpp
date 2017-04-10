#include "message_base.h"
#include "privmsg.h"

#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  namespace message {
    Privmsg::Privmsg(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message) :
      MessageBase::MessageBase("PRIVMSG", channel, timestamp, nick),
      message_(message)
    {
    }

    picojson::object Privmsg::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
