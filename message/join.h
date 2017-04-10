#pragma once

#include "message_base.h"

#include <string>
#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  namespace message {
    class Join : public MessageBase {
    public:
      Join(std::string const& channel, struct tm const& timestamp, std::string const& nick, std::string const& user, std::string const& host);
      virtual ~Join() = default;

    private:
      const std::string user_;
      const std::string host_;

      virtual picojson::object DoToJsonObject() const override;
    };
  }
}
