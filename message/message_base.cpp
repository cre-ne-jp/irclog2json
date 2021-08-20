#include "message_base.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    MessageBase::MessageBase(const char* type, std::string const& channel, struct tm const& timestamp, std::string const& nick) :
      type_(type),
      channel_(channel),
      timestamp_(timestamp),
      nick_(nick)
    {
    }

    MessageBase::~MessageBase() = default;

    picojson::object MessageBase::DoToJsonObject() const {
      picojson::object m;

      char timestamp_str[32];
      strftime(timestamp_str, sizeof(timestamp_str), "%F %T +0900", &timestamp_);

      m["type"] = picojson::value(type_);
      m["channel"] = picojson::value(channel_);
      m["timestamp"] = picojson::value(timestamp_str);
      m["nick"] = picojson::value(nick_);

      return m;
    }
  }
}
