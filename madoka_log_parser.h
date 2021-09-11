#pragma once

#include <memory>

#include "irc_log_parser.h"

#include "basic_irc_log_parser.h"

#include "message/line_converter.h"
#include "message/message_base.h"

namespace irclog2json {

class MadokaLogParser : public IrcLogParser {
public:
  /**
   * @param is IRCログの入力ストリーム。
   * @param line_converter 行解析器。
   */
  MadokaLogParser(std::istream* is,
                  std::unique_ptr<message::LineConverter>&& line_converter);
  virtual ~MadokaLogParser();

  MadokaLogParser(const MadokaLogParser&) = delete;
  MadokaLogParser(MadokaLogParser&&) = delete;
  MadokaLogParser& operator=(const MadokaLogParser&) = delete;
  MadokaLogParser& operator=(MadokaLogParser&&) = delete;

private:
  /** 基本的なIRCログ解析器。 */
  BasicIrcLogParser basic_parser_;

  /** ログ解析およびメッセージ抽出の実装。 */
  virtual std::vector<std::unique_ptr<message::MessageBase>>
  DoExtractMessages() const override;
};

} // namespace irclog2json
