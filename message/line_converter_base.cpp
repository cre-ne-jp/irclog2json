#include "line_converter_base.h"
#include "message_base.h"

#include <string>
#include <regex>
#include <memory>
#include <ctime>

namespace irclog2json {
  namespace message {
    std::string LineConverterBase::RemoveControlCodes(std::string const& s) {
      static const std::regex re(R"([\x02\x03\x1D\x1F\x16\x0F])");
      return std::regex_replace(s, re, "");
    }

    LineConverterBase::LineConverterBase(std::string const& channel, struct tm const& tm_date) :
      channel_(channel),
      tm_date_(tm_date)
    {
    }

    LineConverterBase::~LineConverterBase() = default;

    std::unique_ptr<MessageBase> LineConverterBase::ToMessage(std::string const& line) const {
      return DoToMessage(line);
    }

    void LineConverterBase::SetHMS(struct tm& timestamp, std::smatch const& m) const {
      timestamp = tm_date_;

      timestamp.tm_hour = std::stoi(m[1]);
      timestamp.tm_min = std::stoi(m[2]);
      timestamp.tm_sec = std::stoi(m[3]);
    }
  }
}
