#pragma once

#include "message_base.h"

#include <ctime>
#include <picojson.h>
#include <string>

namespace irclog2json {
namespace message {

/** 基本的なIRCメッセージを表すクラス。 */
class BasicMessage : public MessageBase {
public:
  /**
   * @param type メッセージの種類。
   * @param channel チャンネル名。
   * @param timestamp タイムスタンプ。
   * @param nick ニックネーム。
   * @param message メッセージの内容。
   */
  BasicMessage(const char* type, const std::string& channel,
               const struct tm& timestamp, const std::string& nick,
               const std::string& message);
  virtual ~BasicMessage() = default;

protected:
  /** JSONオブジェクトへの変換処理の実装。 */
  virtual picojson::object DoToJsonObject() const override;

private:
  /** メッセージの内容。 */
  const std::string message_;
};

} // namespace message
} // namespace irclog2json
