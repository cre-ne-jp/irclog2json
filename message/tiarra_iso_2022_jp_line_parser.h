#pragma once

#include <memory>
#include <string>

#include "iso_2022_jp_line_parser.h"

#include "tiarra_line_parser.h"

namespace irclog2json {
namespace message {

/** ISO-2022-JPのTiarraログの行解析器。 */
class TiarraIso2022JpLineParser : public Iso2022JpLineParser {
public:
  TiarraIso2022JpLineParser(
      std::unique_ptr<TiarraLineParser>&& utf8_line_parser);
  virtual ~TiarraIso2022JpLineParser();

private:
  /**
   * @brief ISO-2022-JPの文字集合をASCIIで終わらせる処理の実装。
   * @param line 文字集合を修正する行。
   * @return 修正結果。
   */
  virtual Iso2022JpCharsetFixResult
  DoForceToEndInASCII(const std::string& line) const override;
};

} // namespace message
} // namespace irclog2json
