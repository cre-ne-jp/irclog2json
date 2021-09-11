#pragma once

#include <ctime>
#include <memory>
#include <string>

#include "message_base.h"
#include "utf8_line_parser.h"

namespace irclog2json {
namespace message {

/** Tiarraログの行解析器。 */
class TiarraLineParser : public UTF8LineParser {
public:
  using UTF8LineParser::UTF8LineParser;
  virtual ~TiarraLineParser();

private:
  virtual std::unique_ptr<MessageBase>
  DoToMessage(const std::string& line) const override;
};

} // namespace message
} // namespace irclog2json
