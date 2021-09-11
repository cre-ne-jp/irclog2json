#pragma once

#include <memory>
#include <string>

#include "line_converter.h"
#include "utf8_line_converter.h"

namespace irclog2json {
namespace message {

/** ISO-2022-JPの文字集合をASCIIで終わらせる処理の結果を表す構造体。 */
struct Iso2022JpCharsetFixResult {
  /** 文字集合が修正された行。 */
  std::string fixed_line;
  /** 文字集合が修正されたか。 */
  bool charset_fixed;
};

/** ISO-2022-JPのIRCログの行解析器の抽象クラス。 */
class Iso2022JpLineParser : public LineParser {
public:
  /**
   * @param utf8_line_parser UTF-8の行解析器。
   */
  Iso2022JpLineParser(std::unique_ptr<UTF8LineParser>&& utf8_line_parser);

  virtual ~Iso2022JpLineParser();

  /**
   * @brief ISO-2022-JPの文字集合がASCIIで終わるようにする。
   * @param line 文字集合を修正する行。
   * @return 文字集合が修正された行、修正が行われたか。
   */
  Iso2022JpCharsetFixResult ForceToEndInAscii(const std::string& line) const {
    return DoForceToEndInASCII(line);
  }

protected:
  static constexpr const char* SwitchToJISX0208 = "\x1B\x24\x42";
  static constexpr const char* SwitchToASCII = "\x1B\x28\x42";

  /**
   * @brief ISO-2022-JPの文字集合がASCIIで終わっているか判定する。
   * @param line 判定する行の文字列。
   * @retval true 文字集合がASCIIで終わっている場合。
   * @retval false 文字集合がASCII以外で終わっている場合。
   */
  bool EndInASCII(const std::string& line) const;

private:
  /** UTF-8の行解析器。 */
  std::unique_ptr<UTF8LineParser> const utf8_line_parser_;

  /**
   * @brief IRCログの行のメッセージオブジェクトへの変換の実装。
   * @param line IRCログの行。
   * @return 変換されたメッセージオブジェクト。
   *
   * 行をUTF-8に変換した後、メッセージオブジェクトに変換する。
   */
  virtual std::unique_ptr<MessageBase>
  DoToMessage(const std::string& line) const override;

  /**
   * @brief ISO-2022-JPの文字集合をASCIIで終わらせる処理の実装。
   * @param line 文字集合を修正する行。
   * @return 文字集合が修正された行、修正が行われたか。
   */
  virtual Iso2022JpCharsetFixResult
  DoForceToEndInASCII(const std::string& line) const = 0;
};

} // namespace message
} // namespace irclog2json
