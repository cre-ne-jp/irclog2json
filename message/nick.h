#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class Nick : public MessageBase {
    public:
      Nick(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& new_nick);
      virtual ~Nick() = default;

    private:
      const std::string new_nick_;

      virtual picojson::object DoToJsonObject() const override;
    };
  }
}
