#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include <picojson.h>

#include "irc_log_parser.h"

#include "message/line_converter.h"
#include "message/message_base.h"

namespace irclog2json {

/** 基本的なIRCログ解析器。 */
class BasicIrcLogParser : public IrcLogParser {
public:
  /**
   * @param is IRCログの入力ストリーム。
   * @param line_converter 行解析器。
   */
  BasicIrcLogParser(std::istream* is,
                    std::unique_ptr<message::LineConverter>&& line_converter);
  virtual ~BasicIrcLogParser();

  BasicIrcLogParser(const BasicIrcLogParser&) = delete;
  BasicIrcLogParser(BasicIrcLogParser&&) = delete;
  BasicIrcLogParser& operator=(const BasicIrcLogParser&) = delete;
  BasicIrcLogParser& operator=(BasicIrcLogParser&&) = delete;

private:
  /** IRCログの入力ストリーム。 */
  std::istream* const is_;
  /** 行解析器。 */
  std::unique_ptr<message::LineConverter> line_converter_;

  /** ログ解析およびメッセージ抽出の実装。 */
  virtual std::vector<std::unique_ptr<message::MessageBase>>
  DoExtractMessages() const override;
};

} // namespace irclog2json
