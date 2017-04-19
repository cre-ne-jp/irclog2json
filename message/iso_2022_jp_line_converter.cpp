#include "iso_2022_jp_line_converter.h"
#include "line_converter_base.h"
#include "message_base.h"

#include <vector>
#include <string>
#include <memory>
#include <unicode/unistr.h>

namespace {
  std::string JisToUtf8(std::string const& s);

  std::string JisToUtf8(std::string const& s) {
    icu::UnicodeString src(s.c_str(), "iso-2022-jp");
    size_t length = src.extract(0, src.length(), nullptr, "utf8");

    std::vector<char> result(length + 1);
    src.extract(0, src.length(), &result[0], "utf8");

    return std::string(result.begin(), result.end() - 1);
  }
}

namespace irclog2json {
  namespace message {
    Iso2022JpLineConverter::Iso2022JpLineConverter(std::string const& channel,
                                                   struct tm const& tm_date,
                                                   LineConverterBase const& line_converter) :
      LineConverterBase::LineConverterBase(channel, tm_date),
      line_converter_(line_converter)
    {
    }

    Iso2022JpLineConverter::~Iso2022JpLineConverter() = default;

    std::unique_ptr<MessageBase> Iso2022JpLineConverter::DoToMessage(std::string const& line) const {
      std::string line_utf8 = JisToUtf8(line);
      return line_converter_.ToMessage(line_utf8);
    }
  }
}
