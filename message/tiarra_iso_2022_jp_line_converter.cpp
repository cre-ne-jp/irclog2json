#include <memory>
#include <regex>
#include <string>

#include "iso_2022_jp_line_converter.h"
#include "tiarra_iso_2022_jp_line_converter.h"
#include "tiarra_re.h"

namespace irclog2json {
namespace message {

TiarraIso2022JpLineConverter::TiarraIso2022JpLineConverter(
    std::unique_ptr<TiarraLineConverter>&& utf8_line_converter)
    : Iso2022JpLineConverter::Iso2022JpLineConverter{std::move(utf8_line_converter)} {
}

TiarraIso2022JpLineConverter::~TiarraIso2022JpLineConverter() = default;

Iso2022JpCharsetFixResult TiarraIso2022JpLineConverter::DoForceToEndInASCII(
    const std::string& line) const {
  // TODO: 修正処理を書く
  return Iso2022JpCharsetFixResult{line, false};
}

} // namespace message
} // namespace irclog2json
