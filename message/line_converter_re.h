#pragma once

#define RE_LETTER "[A-Za-z]"
#define RE_SPECIAL R"([\x5B-\x60\x7B-\x7D])"
// RFC 2812 より寛容
#define NICK_PATTERN "(?:" RE_LETTER R"(|\d|)" RE_SPECIAL "|-)+"
#define USER_PATTERN R"([^\x00\x0A\x0D\x20\x40]+)"
#define RE_CHANSTRING R"([^\x00\x07\x0A\x0D\x20,:]+)"
#define CHANNEL_PATTERN "[#+&]" RE_CHANSTRING

#define RE_IP4ADDR R"((?:\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))"
// ホスト名はとりあえず IPv4 のみ
#define RE_HOSTADDR RE_IP4ADDR
#define RE_SHORTNAME "(?:" RE_LETTER R"(|\d)(?:)" RE_LETTER R"(|\d|-)*(?:)" RE_LETTER R"(|\d)*)"
#define RE_HOSTNAME RE_SHORTNAME R"((?:\.)" RE_SHORTNAME R"()*\.?)"
#define HOST_PATTERN "(?:" RE_HOSTNAME "|" RE_HOSTADDR ")"
#define TARGET_PATTERN "(?:" NICK_PATTERN "|" HOST_PATTERN ")"
