#include "message_base.h"
#include "notice.h"

#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  namespace message {
    Notice::Notice(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message) :
      MessageBase::MessageBase("NOTICE", channel, timestamp, nick),
      message_(message)
    {
    }

    picojson::object Notice::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(message_);

      return m;
    }
  }
}
