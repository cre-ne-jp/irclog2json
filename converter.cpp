#include <fstream>
#include <iostream>
#include <memory>
#include <picojson.h>
#include <string>

#include "converter.h"

#include "message/line_converter.h"
#include "message/message_base.h"

namespace irclog2json {

Converter::Converter(std::ifstream* const f,
                     std::unique_ptr<message::LineConverter>&& line_converter)
    : f_{f}, line_converter_{std::move(line_converter)} {
}

Converter::~Converter() = default;

std::vector<std::unique_ptr<message::MessageBase>>
Converter::ExtractMessages() const {
  std::vector<std::unique_ptr<message::MessageBase>> messages;

  std::string line;
  while (std::getline(*f_, line)) {
    if (!line.empty()) {
      std::unique_ptr<message::MessageBase> message =
          line_converter_->ToMessage(line);
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
