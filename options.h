#pragma once

namespace irclog2json {
  /** @brief オプション設定を示す構造体。 */
  struct Options {
  public:
    /** @brief ログの書式を示す列挙型。 */
    enum class LogFormat {
      /** @brief 未指定。 */
      NotSpecified,
      /** @brief Tiarraが出力したログ。 */
      Tiarra,
      /** @brief madokaが出力したログ。 */
      Madoka
    };

    /** @brief ログの書式。 */
    LogFormat log_format;
    /** @brief 文字コードがISO-2022-JPか。 */
    bool iso_2022_jp;
    /** @brief 見やすく出力するか。 */
    bool pretty;

    /** @brief オプション設定を初期化する。 */
    Options() :
      log_format(LogFormat::NotSpecified),
      iso_2022_jp(false),
      pretty(false)
    {
    }

    ~Options() = default;

    Options(Options const&) = default;
    Options(Options&&) = default;
    Options& operator =(Options const&) = default;
    Options& operator =(Options&&) = default;
  };
}
