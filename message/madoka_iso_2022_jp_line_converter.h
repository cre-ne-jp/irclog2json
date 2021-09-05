#pragma once

#include <memory>
#include <string>

#include "iso_2022_jp_line_converter.h"

#include "madoka_log_line_converter.h"

namespace irclog2json {
namespace message {

/** ISO-2022-JPのMadokaログの行変換器。 */
class MadokaIso2022JpLineConverter : public Iso2022JpLineConverter {
public:
  MadokaIso2022JpLineConverter(
      std::unique_ptr<MadokaLineConverter>&& utf_8_line_converter);
  virtual ~MadokaIso2022JpLineConverter();

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
