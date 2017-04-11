#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class Privmsg : public MessageBase {
    public:
      Privmsg(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message);
      virtual ~Privmsg() = default;

    private:
      const std::string message_;

      virtual picojson::object DoToJsonObject() const override;
    };
  }
}
