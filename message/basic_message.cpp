#include "message_base.h"
#include "basic_message.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    BasicMessage::BasicMessage(const char* type, std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message) :
      MessageBase::MessageBase(type, channel, timestamp, nick),
      message_(message)
    {
    }

    picojson::object BasicMessage::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
