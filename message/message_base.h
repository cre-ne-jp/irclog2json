#pragma once

#include <ctime>
#include <picojson.h>
#include <string>

namespace irclog2json {
namespace message {

/** IRCメッセージの基底クラス。 */
class MessageBase {
public:
  /**
   * @param type メッセージの種類。
   * @param channel チャンネル名。
   * @param timestamp タイムスタンプ。
   * @param nick ニックネーム。
   */
  MessageBase(const char* type, const std::string& channel,
              const struct tm& timestamp, const std::string& nick);
  virtual ~MessageBase();

  MessageBase(MessageBase const&) = delete;
  MessageBase(MessageBase&&) = delete;
  MessageBase& operator=(MessageBase const&) = delete;
  MessageBase& operator=(MessageBase&&) = delete;

  /** JSONオブジェクトに変換する。 */
  picojson::object ToJsonObject() const {
    return DoToJsonObject();
  }

  /** ISO-2022-JPの文字集合が修正されたか示すフラグを設定する。 */
  void SetIso2022JpCharsetFixed(bool value) {
    iso_2022_jp_charset_fixed_ = value;
  }

protected:
  /** JSONオブジェクトへの変換処理の実装。 */
  virtual picojson::object DoToJsonObject() const;

private:
  /** メッセージの種類。 */
  const char* type_;
  /** チャンネル名。 */
  const std::string channel_;
  /** タイムスタンプ。 */
  const struct tm timestamp_;
  /** ニックネーム。 */
  const std::string nick_;

  /** ISO-2022-JPの文字集合が修正されたか。 */
  bool iso_2022_jp_charset_fixed_;
};

} // namespace message
} // namespace irclog2json
