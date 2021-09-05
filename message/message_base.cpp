#include "message_base.h"

#include <ctime>
#include <picojson.h>

namespace irclog2json {
namespace message {
MessageBase::MessageBase(const char* type, const std::string& channel,
                         const struct tm& timestamp, const std::string& nick)
    : type_{type},
      channel_{channel},
      timestamp_{timestamp},
      nick_{nick},
      iso_2022_jp_charset_fixed_{false} {
}

MessageBase::~MessageBase() = default;

picojson::object MessageBase::DoToJsonObject() const {
  picojson::object m;

  char timestamp_str[32];
  std::strftime(timestamp_str, sizeof(timestamp_str), "%F %T +0900",
                &timestamp_);

  m["type"] = picojson::value{type_};
  m["channel"] = picojson::value{channel_};
  m["timestamp"] = picojson::value{timestamp_str};
  m["nick"] = picojson::value{nick_};
  m["iso2022JpCharsetFixed"] = picojson::value{iso_2022_jp_charset_fixed_};

  return m;
}

} // namespace message
} // namespace irclog2json
