#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class BasicMessage : public MessageBase {
    public:
      BasicMessage(const char* type, std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& message);
      virtual ~BasicMessage() = default;

    protected:
      virtual picojson::object DoToJsonObject() const override;

    private:
      const std::string message_;
    };
  }
}

