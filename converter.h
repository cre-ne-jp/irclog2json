#pragma once

#include <fstream>
#include <memory>

#include <picojson.h>

#include "message/line_converter.h"

namespace irclog2json {

class Converter {
public:
  Converter(std::ifstream* f,
            std::unique_ptr<message::LineConverter>&& line_converter);
  ~Converter();

  Converter(Converter const&) = delete;
  Converter(Converter&&) = delete;
  Converter& operator=(Converter const&) = delete;
  Converter& operator=(Converter&&) = delete;

  picojson::value Convert() const;

private:
  std::ifstream* const f_;
  std::unique_ptr<message::LineConverter> line_converter_;
};

} // namespace irclog2json
