#pragma once

#include <ctime>
#include <memory>
#include <regex>
#include <string>

#include "line_converter.h"

namespace irclog2json {
namespace message {

/** UTF-8のIRCログの行変換器の抽象クラス。 */
class UTF8LineConverter : public LineConverter {
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
  UTF8LineConverter(const std::string& channel, const struct tm& tm_date);

  virtual ~UTF8LineConverter() = default;

  UTF8LineConverter(const UTF8LineConverter&) = delete;
  UTF8LineConverter(UTF8LineConverter&&) = delete;
  UTF8LineConverter& operator=(const UTF8LineConverter&) = delete;
  UTF8LineConverter& operator=(UTF8LineConverter&&) = delete;

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
