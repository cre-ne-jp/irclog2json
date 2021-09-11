#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include <picojson.h>

#include "message/line_converter.h"
#include "message/message_base.h"

namespace irclog2json {

/** 基本的なIRCログ解析器。 */
class BasicIrcLogParser {
public:
  BasicIrcLogParser(
      std::istream* f,
      std::unique_ptr<message::LineConverter>&& line_converter);
  ~BasicIrcLogParser();

  BasicIrcLogParser(BasicIrcLogParser const&) = delete;
  BasicIrcLogParser(BasicIrcLogParser&&) = delete;
  BasicIrcLogParser& operator=(BasicIrcLogParser const&) = delete;
  BasicIrcLogParser& operator=(BasicIrcLogParser&&) = delete;

  std::vector<std::unique_ptr<message::MessageBase>> ExtractMessages() const;

private:
  std::istream* const is_;
  std::unique_ptr<message::LineConverter> line_converter_;
};

} // namespace irclog2json
