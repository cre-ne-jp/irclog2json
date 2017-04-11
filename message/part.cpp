#include "message_base.h"
#include "part.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    Part::Part(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message) :
      MessageBase::MessageBase("PART", channel, timestamp, nick),
      message_(message)
    {
    }

    picojson::object Part::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
