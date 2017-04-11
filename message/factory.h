#pragma once

#include "message_base.h"

#include <string>
#include <regex>
#include <memory>
#include <ctime>

namespace irclog2json {
  namespace message {
    class Factory {
    public:
      Factory(std::string const& channel, struct tm const& tm_date);

      std::unique_ptr<MessageBase> ToMessage(std::string const& line) const;

    private:
      const std::string channel_;
      const struct tm tm_date_;

      void SetHMS(struct tm& timestamp, std::smatch const& m) const;
    };
  }
}
