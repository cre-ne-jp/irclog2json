#include <iostream>
#include <memory>
#include <picojson.h>
#include <string>

#include "basic_irc_log_parser.h"

#include "message/line_parser.h"
#include "message/message_base.h"

namespace irclog2json {

BasicIrcLogParser::BasicIrcLogParser(
    std::istream* const is, std::unique_ptr<message::LineParser>&& line_parser)
    : IrcLogParser::IrcLogParser{},
      is_{is},
      line_parser_{std::move(line_parser)} {
}

BasicIrcLogParser::~BasicIrcLogParser() = default;

std::vector<std::unique_ptr<message::MessageBase>>
BasicIrcLogParser::DoExtractMessages() const {
  std::vector<std::unique_ptr<message::MessageBase>> messages;

  std::string line;
  while (std::getline(*is_, line)) {
    if (!line.empty()) {
      std::unique_ptr<message::MessageBase> message =
          line_parser_->ToMessage(line);
      if (message) {
        messages.push_back(std::move(message));
      } else {
        std::cout << "Ignored: " << line << std::endl;
      }
    }
  }

  return messages;
}

} // namespace irclog2json
