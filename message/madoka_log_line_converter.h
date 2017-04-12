#pragma once

#include "line_converter_base.h"

#include <string>
#include <memory>
#include <ctime>

namespace irclog2json {
  namespace message {
    class MadokaLogLineConverter : public LineConverterBase {
    public:
      using LineConverterBase::LineConverterBase;
      virtual ~MadokaLogLineConverter();

    private:
      virtual std::unique_ptr<MessageBase> DoToMessage(std::string const& line) const override;
    };
  }
}
