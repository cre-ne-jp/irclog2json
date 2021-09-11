#pragma once

#include <memory>
#include <vector>

#include "message/message_base.h"

namespace irclog2json {

/** IRCログ解析器の抽象クラス。 */
class IrcLogParser {
public:
  IrcLogParser() = default;
  virtual ~IrcLogParser() = default;

  IrcLogParser(const IrcLogParser&) = delete;
  IrcLogParser(IrcLogParser&&) = delete;
  IrcLogParser& operator=(const IrcLogParser&) = delete;
  IrcLogParser&& operator=(IrcLogParser&&) = delete;

  /**
   * @brief IRCログを解析してメッセージを抽出する。
   * @return IRCメッセージの配列。
   */
  std::vector<std::unique_ptr<message::MessageBase>> ExtractMessages() const {
    return DoExtractMessages();
  }

private:
  /**
   * @brief IRCログ解析およびメッセージ抽出の実装。
   * @return IRCメッセージの配列。
   */
  virtual std::vector<std::unique_ptr<message::MessageBase>>
  DoExtractMessages() const = 0;
};

} // namespace irclog2json
