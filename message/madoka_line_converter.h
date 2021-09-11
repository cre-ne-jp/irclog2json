#pragma once

#include <ctime>
#include <memory>
#include <string>

#include "message_base.h"
#include "utf8_line_converter.h"

namespace irclog2json {
namespace message {

/** Madokaログの行解析器。 */
class MadokaLineParser : public UTF8LineParser {
public:
  using UTF8LineParser::UTF8LineParser;
  virtual ~MadokaLineParser();

private:
  virtual std::unique_ptr<MessageBase>
  DoToMessage(std::string const& line) const override;
};

} // namespace message
} // namespace irclog2json
