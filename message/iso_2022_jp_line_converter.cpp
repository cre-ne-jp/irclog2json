#include <memory>
#include <string>
#include <vector>

#include <unicode/unistr.h>

#include "message_base.h"
#include "utf8_line_converter.h"

#include "iso_2022_jp_line_converter.h"

namespace {
/**
 * @brief mIRCの書式設定解除コードをエスケープする。
 * @param [in] s 変換する文字列。
 * @param [out] plain_code_indices "^O"
 * が何文字目にあったかを記録するための配列。
 * @return 変換後の文字列。
 *
 * mIRCの書式設定解除コード "^O" はISO-2022-JPのシフトイン（SI）と
 * 同じ文字のため、UTF-8への変換時に文字化けの原因となる。
 * この文字を無害な "^Z"（置換）に変換し、後の復元用に
 * 何文字目に "^O" があったかを配列に格納する。
 *
 * 文字数を数える際は、ISO-2022-JPの文字集合切り替えを検知し、
 * ASCIIの場合は1バイト1文字、JIS X 0208の場合は2バイト1文字となるように
 * 数える。
 */
std::string EscapeMIrcPlain(const std::string& s,
                            std::vector<size_t>& plain_code_indices);

/**
 * @brief mIRCの書式設定解除のエスケープを元に戻す。
 * @param s 変換する文字列。
 * @param plain_code_indices "^O" が何文字目にあったかを記録した配列。
 *
 * 文字の場所の情報を参考にして "^Z" にエスケープされた "^O" を元に戻す。
 */
void UnescapeMIrcPlain(icu::UnicodeString& s,
                       std::vector<size_t> const& plain_code_indices);

/** ISO-2022-JPの文字集合を表す列挙型。 */
enum class Iso2022JpCharset {
  /** ASCII。 */
  Ascii,
  /** JIS-X-0208 */
  JisX0208
};

constexpr char CC_ESC = '\x1B';
constexpr char CC_SUB = '\x1A';
constexpr char CC_EM = '\x19';

std::string EscapeMIrcPlain(std::string const& s,
                            std::vector<size_t>& plain_code_indices) {
  std::string result = s;
  Iso2022JpCharset charset = Iso2022JpCharset::Ascii;
  auto it_end = result.end();

  size_t count = -1;
  for (auto it = result.begin(); it != it_end; ++it) {
    bool change_charset = false;
    char c = *it;

    if (charset == Iso2022JpCharset::JisX0208) {
      if (c == CC_ESC) { // 0x1B
        auto it_1 = it + 1;
        if (it_1 != it_end && *it_1 == '(') { // 0x28
          auto it_2 = it_1 + 1;
          if (it_2 != it_end && *it_2 == 'B') { // 0x42
            change_charset = true;
            charset = Iso2022JpCharset::Ascii;
            it = it_2;
          }
        }
      }

      if (!change_charset) {
        switch (c) {
        case '\x0E':
          ++count;

          // ^Y に置換する
          *it = CC_EM;
          plain_code_indices.push_back(count);

          break;
        case '\x0F':
          ++count;

          // ^Z に置換する
          *it = CC_SUB;
          plain_code_indices.push_back(count);

          break;
        default: {
          // 2バイトで1文字と数える
          auto it_1 = it + 1;
          if (it_1 != it_end) {
            ++count;
            it = it_1;
          }

          break;
        }
        }
      }
    } else {
      if (c == CC_ESC) { // 0x1B
        auto it_1 = it + 1;
        if (it_1 != it_end && *it_1 == '$') { // 0x24
          auto it_2 = it_1 + 1;
          if (it_2 != it_end && *it_2 == 'B') { // 0x42
            change_charset = true;
            charset = Iso2022JpCharset::JisX0208;
            it = it_2;
          }
        }
      }

      if (!change_charset) {
        ++count;

        switch (c) {
        case '\x0E':
          // ^Y に置換する
          *it = CC_EM;
          plain_code_indices.push_back(count);

          break;
        case '\x0F':
          // ^Z に置換する
          *it = CC_SUB;
          plain_code_indices.push_back(count);

          break;
        default:
          break;
        }
      }
    }
  }

  return result;
}

void UnescapeMIrcPlain(icu::UnicodeString& s,
                       const std::vector<size_t>& plain_code_indices) {
  for (auto i : plain_code_indices) {
    switch (s[i]) {
    case static_cast<char16_t>(CC_EM):
      s.replace(i, 1, u'\u000E');
      break;
    case static_cast<char16_t>(CC_SUB):
      s.replace(i, 1, u'\u000F');
      break;
    default:
      break;
    }
  }
}

} // namespace

namespace irclog2json {
namespace message {

Iso2022JpLineConverter::Iso2022JpLineConverter(
    std::unique_ptr<UTF8LineConverter>&& utf8_line_converter)
    : utf8_line_converter_{std::move(utf8_line_converter)} {
}

Iso2022JpLineConverter::~Iso2022JpLineConverter() = default;

std::unique_ptr<MessageBase>
Iso2022JpLineConverter::DoToMessage(const std::string& line) const {
  std::vector<size_t> plain_code_indices;

  std::string escaped = EscapeMIrcPlain(line, plain_code_indices);
  Iso2022JpCharsetFixResult charset_fix_result = ForceToEndInAscii(escaped);
  icu::UnicodeString escaped_unicode{charset_fix_result.fixed_line.c_str(),
                                     "iso-2022-jp"};
  UnescapeMIrcPlain(escaped_unicode, plain_code_indices);

  std::string unescaped;
  escaped_unicode.toUTF8String(unescaped);

  std::unique_ptr<MessageBase> message =
      utf8_line_converter_->ToMessage(unescaped);
  if (!message) {
    return message;
  }

  message->SetIso2022JpCharsetFixed(charset_fix_result.charset_fixed);

  return message;
}

bool Iso2022JpLineConverter::EndInASCII(const std::string& line) const {
  std::size_t last_jisx0208_pos = line.rfind(SwitchToJISX0208);
  std::size_t last_ascii_pos = line.rfind(SwitchToASCII);

  if (last_jisx0208_pos == std::string::npos) {
    // ASCII文字のみ
    return true;
  }

  return last_ascii_pos != std::string::npos &&
         last_ascii_pos > last_jisx0208_pos;
}

} // namespace message
} // namespace irclog2json
