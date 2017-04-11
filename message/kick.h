#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class Kick : public MessageBase {
    public:
      Kick(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& target, std::string const& message);
      virtual ~Kick() = default;

    private:
      const std::string target_;
      const std::string message_;

      virtual picojson::object DoToJsonObject() const override;
    };
  }
}
