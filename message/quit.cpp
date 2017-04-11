#include "message_base.h"
#include "quit.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    Quit::Quit(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message) :
      MessageBase::MessageBase("QUIT", channel, timestamp, nick),
      message_(message)
    {
    }

    picojson::object Quit::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
