#include "message_base.h"
#include "nick.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    Nick::Nick(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& new_nick) :
      MessageBase::MessageBase("NICK", channel, timestamp, nick),
      new_nick_(new_nick)
    {
    }

    picojson::object Nick::DoToJsonObject() const {
      picojson::object m = MessageBase::DoToJsonObject();

      m["message"] = picojson::value(new_nick_);

      return m;
    }
  }
}
