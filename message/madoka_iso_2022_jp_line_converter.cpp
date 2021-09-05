#include <memory>
#include <regex>
#include <string>

#include "iso_2022_jp_line_converter.h"
#include "madoka_iso_2022_jp_line_converter.h"
#include "madoka_re.h"

namespace irclog2json {
namespace message {

MadokaIso2022JpLineConverter::MadokaIso2022JpLineConverter(
    std::unique_ptr<MadokaLineConverter>&& utf_8_line_converter)
    : Iso2022JpLineConverter::Iso2022JpLineConverter{std::move(utf_8_line_converter)} {
}

MadokaIso2022JpLineConverter::~MadokaIso2022JpLineConverter() = default;

Iso2022JpCharsetFixResult MadokaIso2022JpLineConverter::DoForceToEndInASCII(
    const std::string& line) const {
  std::smatch m;

  if (std::regex_match(line, m, madoka::RePart) ||
      std::regex_match(line, m, madoka::ReKick)) {
    if (EndInASCII(line)) {
      return Iso2022JpCharsetFixResult{line, false};
    }

    std::string line_end_in_ascii{line};
    line_end_in_ascii.insert(line.length() - 1, SwitchToASCII);

    return Iso2022JpCharsetFixResult{line_end_in_ascii, true};
  }

  if (std::regex_match(line, m, madoka::ReQuit)) {
    if (EndInASCII(line)) {
      return Iso2022JpCharsetFixResult{line, false};
    }

    std::string line_end_in_ascii{line};
    std::size_t len = line.length();
    std::size_t insert_pos = (line[len - 2] == '"') ? (len - 2) : (len - 1);

    line_end_in_ascii.insert(insert_pos, SwitchToASCII);

    return Iso2022JpCharsetFixResult{line_end_in_ascii, true};
  }

  return Iso2022JpCharsetFixResult{line, false};
}

} // namespace message
} // namespace irclog2json
