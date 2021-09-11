#include <memory>
#include <regex>
#include <string>

#include "iso_2022_jp_line_parser.h"
#include "tiarra_iso_2022_jp_line_parser.h"
#include "tiarra_re.h"

namespace irclog2json {
namespace message {

TiarraIso2022JpLineParser::TiarraIso2022JpLineParser(
    std::unique_ptr<TiarraLineParser>&& utf8_line_parser)
    : Iso2022JpLineParser::Iso2022JpLineParser{std::move(utf8_line_parser)} {
}

TiarraIso2022JpLineParser::~TiarraIso2022JpLineParser() = default;

Iso2022JpCharsetFixResult
TiarraIso2022JpLineParser::DoForceToEndInASCII(const std::string& line) const {
  // TODO: 修正処理を書く
  return Iso2022JpCharsetFixResult{line, false};
}

} // namespace message
} // namespace irclog2json
