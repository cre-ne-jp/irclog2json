#pragma once

#include "line_converter_base.h"

#include <string>
#include <memory>

namespace irclog2json {
  namespace message {
    class Iso2022JpLineConverter : public LineConverterBase {
    public:
      Iso2022JpLineConverter(std::string const& channel,
                             struct tm const& tm_date,
                             LineConverterBase const& line_converter);
      virtual ~Iso2022JpLineConverter();

    private:
      LineConverterBase const& line_converter_;

      virtual std::unique_ptr<MessageBase> DoToMessage(std::string const& line) const override;
    };
  }
}
