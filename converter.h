#pragma once

#include <string>
#include <fstream>
#include <ctime>
#include <picojson.h>

namespace irclog2json {
  namespace message {
    class LineConverterBase;
  }

  class Converter {
  public:
    Converter(std::ifstream& f,
              message::LineConverterBase const& line_converter);
    ~Converter() = default;

    Converter(Converter const&) = delete;
    Converter(Converter&&) = delete;
    Converter& operator =(Converter const&) = delete;
    Converter& operator =(Converter&&) = delete;

    picojson::value Convert() const;

  private:
    std::ifstream& f_;
    message::LineConverterBase const& line_converter_;
  };
}
