#include <ctime>
#include <string>

#include "madoka_auto_down.h"

namespace irclog2json {
namespace message {

MadokaAutoDown::MadokaAutoDown(const std::string& channel,
                               const struct tm& timestamp)
    : BasicMessage::BasicMessage{"QUIT", channel, timestamp, "bot",
                                 R"("auto down")"} {
}

MadokaAutoDown::~MadokaAutoDown() = default;

} // namespace message
} // namespace irclog2json
