#include "message_base.h"
#include "topic.h"

#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  namespace message {
    Topic::Topic(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message) :
      MessageBase::MessageBase("TOPIC", channel, timestamp, nick),
      message_(message)
    {
    }

    picojson::object Topic::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
