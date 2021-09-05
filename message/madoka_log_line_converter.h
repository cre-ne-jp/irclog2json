#pragma once

#include <ctime>
#include <memory>
#include <string>

#include "message_base.h"
#include "utf8_line_converter.h"

namespace irclog2json {
namespace message {

/** Madokaログの行変換器。 */
class MadokaLineConverter : public UTF8LineConverter {
public:
  using UTF8LineConverter::UTF8LineConverter;
  virtual ~MadokaLineConverter();

private:
  virtual std::unique_ptr<MessageBase>
  DoToMessage(std::string const& line) const override;
};

} // namespace message
} // namespace irclog2json
