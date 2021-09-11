#include <memory>

#include "madoka_log_parser.h"

#include "irc_log_parser.h"

#include "basic_irc_log_parser.h"

#include "message/join.h"
#include "message/madoka_auto_down.h"
#include "message/message_base.h"

namespace irclog2json {

using message::Join;
using message::MadokaAutoDown;
using message::MessageBase;

MadokaLogParser::MadokaLogParser(
    std::istream* is, std::unique_ptr<message::LineParser>&& line_parser)
    : basic_parser_{is, std::move(line_parser)} {
}

MadokaLogParser::~MadokaLogParser() = default;

namespace {

/**
 * @brief 出現後最初のJOINを利用してAutoDownのニックネームを設定する。
 * @param messages IRCメッセージの配列。
 */
void SetNickOfAutoDownsWithFirstJoin(
    std::vector<std::unique_ptr<MessageBase>>* messages);

} // namespace

std::vector<std::unique_ptr<message::MessageBase>>
MadokaLogParser::DoExtractMessages() const {

  std::vector<std::unique_ptr<MessageBase>> messages =
      basic_parser_.ExtractMessages();

  SetNickOfAutoDownsWithFirstJoin(&messages);

  return messages;
}

namespace {

void SetNickOfAutoDownsWithFirstJoin(
    std::vector<std::unique_ptr<MessageBase>>* messages) {
  MadokaAutoDown* auto_down = nullptr;
  Join* join = nullptr;

  for (auto& m : *messages) {
    if (auto_down) {
      join = dynamic_cast<Join*>(m.get());
      if (join) {
        auto_down->SetNick(join->GetNick());
        auto_down = nullptr;
      }
    } else {
      auto_down = dynamic_cast<MadokaAutoDown*>(m.get());
    }
  }
}

} // namespace

} // namespace irclog2json
