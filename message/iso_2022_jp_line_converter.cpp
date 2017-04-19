#include "iso_2022_jp_line_converter.h"
#include "line_converter_base.h"
#include "message_base.h"

#include <vector>
#include <string>
#include <codecvt>
#include <memory>
#include <unicode/unistr.h>

namespace {
  /**
   * @brief 文字コードをISO-2022-JPからUTF-8に変換する。
   * @param s 変換する文字列。
   * @return 変換後の文字列。
   */
  std::string Iso2022JpToUtf8(std::string const& s);

  /**
   * @brief mIRCの書式設定解除コードをエスケープする。
   * @param [in] s 変換する文字列。
   * @param [out] plain_code_indices "^O" が何文字目にあったかを記録するための配列。
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
  std::string EscapeMIrcPlain(std::string const& s, std::vector<size_t>& plain_code_indices);

  /**
   * @brief mIRCの書式設定解除のエスケープを元に戻す。
   * @param s 変換する文字列。
   * @param plain_code_indices "^O" が何文字目にあったかを記録した配列。
   * @return 変換後の文字列。
   *
   * 文字の場所の情報を参考にして "^Z" にエスケープされた "^O" を元に戻す。
   */
  std::string UnescapeMIrcPlain(std::string const& s, std::vector<size_t> const& plain_code_indices);

  std::string Iso2022JpToUtf8(std::string const& s) {
    icu::UnicodeString src(s.c_str(), "iso-2022-jp");
    size_t length = src.extract(0, src.length(), nullptr, "utf8");

    std::vector<char> result(length + 1);
    src.extract(0, src.length(), &result[0], "utf8");

    return std::string(result.begin(), result.end() - 1);
  }

  enum class Iso2022JpState {
    Ascii,
    JisX0208
  };

  std::string EscapeMIrcPlain(std::string const& s, std::vector<size_t>& plain_code_indices) {
    std::string result = s;
    Iso2022JpState state = Iso2022JpState::Ascii;
    auto it_end = result.end();

    size_t count = -1;
    for (auto it = result.begin(); it != it_end; ++it) {
      bool change_state = false;
      char c = *it;

      if (state == Iso2022JpState::JisX0208) {
        if (c == 0x1B) {
          auto it_1 = it + 1;
          if (it_1 != it_end && *it_1 == 0x28) {
            auto it_2 = it_1 + 1;
            if (it_2 != it_end && *it_2 == 0x42) {
              change_state = true;
              state = Iso2022JpState::Ascii;
              it = it_2;
            }
          }
        }

        if (!change_state) {
          if (c == 0x0F) {
            ++count;

            // ^Z に置換する
            *it = 0x1A;
            plain_code_indices.push_back(count);
          } else {
            // 2バイトで1文字と数える
            auto it_1 = it + 1;
            if (it_1 != it_end) {
              ++count;
              it = it_1;
            }
          }
        }
      } else {
        if (c == 0x1B) {
          auto it_1 = it + 1;
          if (it_1 != it_end && *it_1 == 0x24) {
            auto it_2 = it_1 + 1;
            if (it_2 != it_end && *it_2 == 0x42) {
              change_state = true;
              state = Iso2022JpState::JisX0208;
              it = it_2;
            }
          }
        }

        if (!change_state) {
          ++count;

          if (c == 0x0F) {
            // ^Z に置換する
            *it = 0x1A;
            plain_code_indices.push_back(count);
          }
        }
      }
    }

    return result;
  }

  std::string UnescapeMIrcPlain(std::string const& s, std::vector<size_t> const& plain_code_indices) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32s = converter.from_bytes(s);

    for (auto i : plain_code_indices) {
      if (u32s[i] == U'\u001A') {
        u32s[i] = U'\u0001';
      }
    }

    return converter.to_bytes(u32s);
  }
}

namespace irclog2json {
  namespace message {
    Iso2022JpLineConverter::Iso2022JpLineConverter(std::string const& channel,
                                                   struct tm const& tm_date,
                                                   LineConverterBase const& line_converter) :
      LineConverterBase::LineConverterBase(channel, tm_date),
      line_converter_(line_converter)
    {
    }

    Iso2022JpLineConverter::~Iso2022JpLineConverter() = default;

    std::unique_ptr<MessageBase> Iso2022JpLineConverter::DoToMessage(std::string const& line) const {
      std::vector<size_t> plain_code_indices;

      std::string escaped = EscapeMIrcPlain(line, plain_code_indices);
      std::string line_utf8 = Iso2022JpToUtf8(escaped);
      std::string unescaped = UnescapeMIrcPlain(line_utf8, plain_code_indices);

      return line_converter_.ToMessage(unescaped);
    }
  }
}
