#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class Part : public MessageBase {
    public:
      Part(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message);
      virtual ~Part() = default;

    private:
      const std::string message_;

      virtual picojson::object DoToJsonObject() const override;
    };
  }
}
