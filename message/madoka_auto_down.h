#pragma once

#include <ctime>
#include <string>

#include <picojson.h>

#include "basic_message.h"

namespace irclog2json {
namespace message {

/**
 * @brief Madokaログのauto downを表すクラス。
 *
 * JSONへ変換する際は、ニックネーム固定、メッセージ固定のQUITに変える。
 */
class MadokaAutoDown : public BasicMessage {
public:
  /**
   * @param channel チャンネル名。
   * @param timestamp タイムスタンプ。
   */
  MadokaAutoDown(const std::string& channel, const struct tm& timestamp);
  virtual ~MadokaAutoDown();
};

} // namespace message
} // namespace irclog2json
