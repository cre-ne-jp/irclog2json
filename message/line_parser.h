#pragma once

#include <memory>
#include <string>

#include "message_base.h"

namespace irclog2json {
namespace message {

/** IRCログ行解析器の抽象クラス。 */
class LineParser {
public:
  LineParser() = default;
  virtual ~LineParser() = default;

  LineParser(const LineParser&) = delete;
  LineParser(LineParser&&) = delete;
  LineParser& operator=(const LineParser&) = delete;
  LineParser& operator=(LineParser&&) = delete;

  /**
   * @brief IRCログの行をメッセージオブジェクトに変換する。
   * @param line IRCログの行。
   * @return 変換されたメッセージオブジェクト。
   */
  std::unique_ptr<MessageBase> ToMessage(const std::string& line) const {
    return DoToMessage(line);
  }

private:
  /**
   * @brief IRCログの行のメッセージオブジェクトへの変換の実装。
   * @param line IRCログの行。
   * @return 変換されたメッセージオブジェクト。
   */
  virtual std::unique_ptr<MessageBase>
  DoToMessage(const std::string& line) const = 0;
};

} // namespace message
} // namespace irclog2json
