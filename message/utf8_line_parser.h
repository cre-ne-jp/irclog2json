#pragma once

#include <ctime>
#include <memory>
#include <regex>
#include <string>

#include "line_parser.h"

namespace irclog2json {
namespace message {

/** UTF-8のIRCログの行解析器の抽象クラス。 */
class UTF8LineParser : public LineParser {
public:
  /**
   * @brief 制御文字を除去する。
   * @param s 対象文字列。
   * @return 制御文字除去後の文字列。
   */
  static std::string RemoveControlCodes(const std::string& s);

  /**
   * @param channel チャンネル名。
   * @param tm_date ログの日付。
   */
  UTF8LineParser(const std::string& channel, const struct tm& tm_date);

  virtual ~UTF8LineParser() = default;

  UTF8LineParser(const UTF8LineParser&) = delete;
  UTF8LineParser(UTF8LineParser&&) = delete;
  UTF8LineParser& operator=(const UTF8LineParser&) = delete;
  UTF8LineParser& operator=(UTF8LineParser&&) = delete;

protected:
  /** チャンネル名。 */
  const std::string channel_;
  /** ログの日付。 */
  const struct tm tm_date_;

  /**
   * @brief 時分秒の正規表現マッチからタイムスタンプを作る。
   * @param m 時分秒の正規表現マッチ。
   * @return タイムスタンプ。
   */
  struct tm GetTimestamp(const std::smatch& m) const;
};

} // namespace message
} // namespace irclog2json
