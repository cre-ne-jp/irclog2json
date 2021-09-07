#pragma once

#include <ctime>
#include <memory>
#include <string>

#include "message_base.h"
#include "utf8_line_converter.h"

namespace irclog2json {
namespace message {

/** Tiarraログの行変換器。 */
class TiarraLineConverter : public UTF8LineConverter {
public:
  using UTF8LineConverter::UTF8LineConverter;
  virtual ~TiarraLineConverter();

private:
  virtual std::unique_ptr<MessageBase>
  DoToMessage(const std::string& line) const override;
};

} // namespace message
} // namespace irclog2json
