#pragma once

#include <regex>

namespace irclog2json {
namespace message {
namespace madoka {

extern const std::regex RePrivmsg;
extern const std::regex ReNotice;
extern const std::regex ReNick1;
extern const std::regex ReNick2;
extern const std::regex ReJoin;
extern const std::regex RePart;
extern const std::regex ReQuit;
extern const std::regex ReKick;
extern const std::regex ReTopic;

} // namespace madoka
} // namespace message
} // namespace irclog2json
