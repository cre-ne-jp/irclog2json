#include "message_base.h"
#include "join.h"

#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  namespace message {
    Join::Join(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& user, std::string const& host) :
      MessageBase::MessageBase("JOIN", channel, timestamp, nick),
      user_(user),
      host_(host)
    {
    }

    picojson::object Join::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["user"] = picojson::value(user_);
      m["host"] = picojson::value(host_);

      return m;
    }
  }
}
