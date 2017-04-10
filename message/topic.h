#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  namespace message {
    class Topic : public MessageBase {
    public:
      Topic(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message);
      virtual ~Topic() = default;

    private:
      const std::string message_;

      virtual picojson::object DoToJsonObject() const override;
    };
  }
}
