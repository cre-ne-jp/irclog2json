#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <unicode/unistr.h>

#include "message_base.h"
#include "utf8_line_parser.h"

#include "iso_2022_jp_line_parser.h"

namespace {

/** ISO-2022-JPの文字集合を表す列挙型。 */
enum class Iso2022JpCharset {
  /** ASCII。 */
  Ascii,
  /** JIS-X-0208 */
  JisX0208
};

/** mIRC制御文字「リセット」のエスケープ方法を表す構造体。 */
struct MIrcPlainEscapingStrategy {
  /** 変更先の文字集合。 */
  Iso2022JpCharset charset_to_switch;
  /** 1文字あたりのバイト数 - 1。 */
  size_t bytes_per_char_minus_1;
};

/** 文字集合変更シーケンスの消費を試みた結果を表す構造体。 */
struct TryConsumingCharsetSwitchSeqResult {
  /** 文字集合が変更されたか。 */
  bool charset_changed;
  /** 次の文字集合。 */
  Iso2022JpCharset next_charset;
  /** 次の文字インデックス。 */
  size_t next_i;
};

/** 制御文字SUB。 */
constexpr char CC_SUB = '\x1A';
/** 制御文字EM。 */
constexpr char CC_EM = '\x19';

/**
 * mIRCの書式設定解除コード "^O" はISO-2022-JPのシフトイン（SI）と
 * 同じ文字のため、UTF-8への変換時に文字化けの原因となる。
 * この文字を無害な "^Z"（置換）に変換し、後の復元用に
 * 何文字目に "^O" があったかを配列に格納する。
 *
 * 文字数を数える際は、ISO-2022-JPの文字集合切り替えを検知し、
 * ASCIIの場合は1バイト1文字、JIS X 0208の場合は2バイト1文字となるように
 * 数える。
 *
 * @brief mIRCの書式設定解除コードをエスケープする。
 * @param s [in] 変換する文字列。
 * @param plain_code_indices [out] "^O"
 * が何文字目にあったかを記録するための配列。
 * @return 変換後の文字列。
 */
std::string EscapeMIrcPlain(const std::string& s,
                            std::vector<size_t>& plain_code_indices);

/**
 * @brief 文字集合変更シーケンスの消費を試みる。
 * @param s 解析対象文字列。
 * @param i 解析する部分の先頭のインデックス。
 * @param charset_from 現在の文字集合。
 * @param charset_to 変更対象の文字集合。
 */
TryConsumingCharsetSwitchSeqResult
TryConsumingCharsetSwitchSeq(const std::string& s, size_t i,
                             Iso2022JpCharset charset_from,
                             Iso2022JpCharset charset_to);

/**
 * 文字の場所の情報を参考にして、エスケープされた文字を元に戻す。
 *
 * @brief mIRCの書式設定解除のエスケープを元に戻す。
 * @param s 変換する文字列。
 * @param plain_code_indices エスケープされた文字の位置を記録した配列。
 */
void UnescapeMIrcPlain(icu::UnicodeString& s,
                       std::vector<size_t> const& plain_code_indices);

/**
 * @brief 文字列が指定された文字列から始まるか調べる。
 * @param sv 調べる対象の文字列。
 * @param part 当てはめる部分文字列。
 * @return svがpartから始まるか。
 */
constexpr bool StartsWith(std::string_view sv, std::string_view part) noexcept;

} // namespace

namespace irclog2json {
namespace message {

Iso2022JpLineParser::Iso2022JpLineParser(
    std::unique_ptr<UTF8LineParser>&& utf8_line_parser)
    : utf8_line_parser_{std::move(utf8_line_parser)} {
}

Iso2022JpLineParser::~Iso2022JpLineParser() = default;

std::unique_ptr<MessageBase>
Iso2022JpLineParser::DoToMessage(const std::string& line) const {
  std::vector<size_t> plain_code_indices;

  std::string escaped = EscapeMIrcPlain(line, plain_code_indices);
  Iso2022JpCharsetFixResult charset_fix_result = ForceToEndInAscii(escaped);
  icu::UnicodeString escaped_unicode{charset_fix_result.fixed_line.c_str(),
                                     "iso-2022-jp"};
  UnescapeMIrcPlain(escaped_unicode, plain_code_indices);

  std::string unescaped;
  escaped_unicode.toUTF8String(unescaped);

  std::unique_ptr<MessageBase> message =
      utf8_line_parser_->ToMessage(unescaped);
  if (!message) {
    return message;
  }

  message->SetIso2022JpCharsetFixed(charset_fix_result.charset_fixed);

  return message;
}

bool Iso2022JpLineParser::EndInASCII(const std::string& line) const {
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

namespace {

TryConsumingCharsetSwitchSeqResult
TryConsumingCharsetSwitchSeq(const std::string& s, size_t i,
                             Iso2022JpCharset charset_from,
                             Iso2022JpCharset charset_to) {
  static const std::unordered_map<Iso2022JpCharset, std::string>
      CharsetChangeTable{
          {Iso2022JpCharset::JisX0208, "\x1B\x24\x42"},
          {Iso2022JpCharset::Ascii, "\x1B\x28\x42"},
      };

  TryConsumingCharsetSwitchSeqResult result{};

  const auto& charset_switch_seq = CharsetChangeTable.at(charset_to);

  if (StartsWith(&s[i], charset_switch_seq)) {
    result.charset_changed = true;
    result.next_charset = charset_to;
    result.next_i = i + charset_switch_seq.size() - 1;
  } else {
    result.charset_changed = false;
    result.next_charset = charset_from;
    result.next_i = i;
  }

  return result;
}

std::string EscapeMIrcPlain(std::string const& s,
                            std::vector<size_t>& plain_code_indices) {
  static const std::unordered_map<char, char> ReplaceMap{
      {'\x0E', CC_EM},
      {'\x0F', CC_SUB},
  };
  static const auto ReplaceMapCEnd = ReplaceMap.cend();

  static const std::unordered_map<Iso2022JpCharset, MIrcPlainEscapingStrategy>
      StrategyMap{
          {Iso2022JpCharset::Ascii, {Iso2022JpCharset::JisX0208, 0}},
          {Iso2022JpCharset::JisX0208, {Iso2022JpCharset::Ascii, 1}},
      };

  std::string result = s;
  Iso2022JpCharset charset = Iso2022JpCharset::Ascii;
  const MIrcPlainEscapingStrategy* strategy = &StrategyMap.at(charset);

  size_t count = -1;
  for (size_t i = 0, len = s.length(); i < len; ++i) {
    bool charset_changed = false;

    auto r = TryConsumingCharsetSwitchSeq(s, i, charset,
                                          strategy->charset_to_switch);

    charset_changed = r.charset_changed;
    charset = r.next_charset;
    i = r.next_i;

    if (charset_changed) {
      strategy = &StrategyMap.at(charset);
    } else {
      auto it = ReplaceMap.find(s[i]);
      if (it == ReplaceMapCEnd) {
        const size_t& delta_i = strategy->bytes_per_char_minus_1;
        if (i + delta_i < len) {
          ++count;
          i += delta_i;
        }
      } else {
        ++count;
        result[i] = it->second;
        plain_code_indices.push_back(count);
      }
    }
  }

  return result;
}

void UnescapeMIrcPlain(icu::UnicodeString& s,
                       const std::vector<size_t>& plain_code_indices) {
  static const std::unordered_map<char16_t, char16_t> ReplaceMap = {
      {static_cast<char16_t>(CC_EM), u'\u000E'},
      {static_cast<char16_t>(CC_SUB), u'\u000F'},
  };
  static const auto ReplaceMapCEnd = ReplaceMap.cend();

  decltype(ReplaceMap)::const_iterator table_it;
  for (auto i : plain_code_indices) {
    table_it = ReplaceMap.find(s[i]);
    if (table_it != ReplaceMapCEnd) {
      s.replace(i, 1, table_it->second);
    }
  }
}

constexpr bool StartsWith(std::string_view sv, std::string_view part) noexcept {
  return sv.substr(0, part.size()) == part;
}

} // namespace
