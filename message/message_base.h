#pragma once

#include <string>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class MessageBase {
    public:
      MessageBase(const char* type, std::string const& channel, struct tm const& timestamp, std::string const& nick);
      virtual ~MessageBase() = default;

      MessageBase(MessageBase const&) = default;
      MessageBase(MessageBase&&) = default;
      MessageBase& operator =(MessageBase const&) = delete;
      MessageBase& operator =(MessageBase &&) = delete;

      picojson::object ToJsonObject() const {
        return DoToJsonObject();
      }

    protected:
      virtual picojson::object DoToJsonObject() const;

    private:
      const char* type_;
      const std::string channel_;
      const struct tm timestamp_;
      const std::string nick_;
    };
  }
}
