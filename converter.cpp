#include "converter.h"
#include "message/message_base.h"
#include "message/line_converter_base.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  Converter::Converter(std::ifstream& f,
                       message::LineConverterBase const& line_converter) :
    f_(f),
    line_converter_(line_converter)
  {
  }

  picojson::value Converter::Convert() const {
    picojson::array json_lines;

    std::string line;
    while (std::getline(f_, line)) {
      if (!line.empty()) {
        std::unique_ptr<message::MessageBase> message = line_converter_.ToMessage(line);
        if (message) {
          json_lines.emplace_back(message->ToJsonObject());
        } else {
          std::cout << "Ignored: " << line << std::endl;
        }
      }
    }

    return picojson::value(json_lines);
  }
}
