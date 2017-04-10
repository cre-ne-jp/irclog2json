#include "converter.h"
#include "message/message_base.h"
#include "message/factory.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  Converter::Converter(std::ifstream& f, std::string const& channel, struct tm const& tm_date) :
    f_(f),
    channel_(channel),
    tm_date_(tm_date)
  {
  }

  picojson::value Converter::Convert() const {
    message::Factory factory(channel_, tm_date_);

    picojson::array json_lines;

    std::string line;
    while (!f_.eof() && !f_.fail()) {
      std::getline(f_, line);

      if (!line.empty()) {
        std::unique_ptr<message::MessageBase> message = factory.ToMessage(line);
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
