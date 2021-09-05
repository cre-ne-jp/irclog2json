#include "converter.h"
#include "options.h"

#include "message/line_converter.h"

#include "message/iso_2022_jp_line_converter.h"
#include "message/utf8_line_converter.h"

#include "message/tiarra_iso_2022_jp_line_converter.h"
#include "message/tiarra_log_line_converter.h"

#include "message/madoka_iso_2022_jp_line_converter.h"
#include "message/madoka_log_line_converter.h"

#include <memory>

#include <clocale>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <libgen.h>
#include <regex>
#include <string>

#include <picojson.h>

/**
 * @brief コマンドライン引数からオプションを解析する。
 * @param [in] argc コマンドライン引数の個数。
 * @param [in] argv コマンドライン引数の配列。
 * @param [out] options オプションの設定先。
 * @param [out] print_usage 使用法を出力するか。
 * @retval <true> 有効なオプションのみ含まれたいた場合。
 * @retval <false> 無効なオプションが含まれていた場合。
 */
static bool ParseOptions(int argc, char* argv[], irclog2json::Options& options,
                         bool& print_usage);

/**
 * @brief ログをJSONオブジェクトの配列に変換する。
 * @param ifs 入力ファイルストリーム。
 * @param channel チャンネル名。
 * @param tm_date 日付を表すtm構造体。
 * @param options オプション設定。
 * @return 変換結果。JSONオブジェクトの配列。
 */
static picojson::value
ConvertLogToJsonObjects(std::ifstream& ifs, std::string const& channel,
                        struct tm const& tm_date,
                        irclog2json::Options const& options);

/**
 * @brief 使用法を表示する。
 * @param argv0 コマンドライン引数の最初に設定されるプログラム名。
 * @param os 出力ストリーム。
 */
static void PrintUsage(char* argv0, std::ostream& os);

int main(int argc, char* argv[]) {
  char* argv0 = argv[0];

  std::setlocale(LC_ALL, "");

  irclog2json::Options options;
  bool print_usage;
  bool no_invalid_option = ParseOptions(argc, argv, options, print_usage);

  argc -= optind;
  argv += optind;

  if (print_usage) {
    PrintUsage(argv0, no_invalid_option ? std::cout : std::cerr);
    std::exit(no_invalid_option ? 0 : 1);
  }

  if (argc != 2) {
    PrintUsage(argv0, std::cerr);
    std::exit(1);
  }

  if (options.log_format == irclog2json::Options::LogFormat::NotSpecified) {
    std::cerr << "Specify the log format." << std::endl;
    std::exit(1);
  }

  // basename(3) が文字列を変更するので退避する意味も含む
  std::string filename(argv[0]);

  std::string channel(argv[1]);

  // ここで argv[0] は変更される
  char* file_basename = basename(argv[0]);

  // ファイル名から日付を抽出する
  struct tm tm_date;
  char* rest = strptime(file_basename, "%Y%m%d.txt", &tm_date);
  if (rest == NULL || *rest != '\0') {
    std::cerr << filename << ": Invalid date" << std::endl;
    std::exit(1);
  }

  // 出力ファイル名を決める
  std::string out_filename(filename);
  // 末尾の .txt -> .json
  out_filename.replace(out_filename.size() - 3, 3, "json");

  std::ifstream ifs;

  ifs.open(filename);
  if (ifs.fail()) {
    std::perror(filename.c_str());
    std::exit(1);
  }

  picojson::value result =
      ConvertLogToJsonObjects(ifs, channel, tm_date, options);

  ifs.close();

  std::ofstream ofs;

  ofs.open(out_filename);
  if (ofs.fail()) {
    std::perror(out_filename.c_str());
    std::exit(1);
  }

  ofs << result.serialize(options.pretty);

  ofs.close();

  std::cout << "Output " << out_filename << '.' << std::endl;

  return 0;
}

static bool ParseOptions(int argc, char* argv[], irclog2json::Options& options,
                         bool& print_usage) {
  constexpr struct option long_options[] = {
      {"tiarra", no_argument, nullptr, 't'},
      {"madoka", no_argument, nullptr, 'm'},
      {"iso-2022-jp", no_argument, nullptr, 'j'},
      {"pretty", no_argument, nullptr, 'p'},
      {"help", no_argument, nullptr, 'h'},
      {0, 0, 0, 0}};
  const char* optstring = "tmjph";

  print_usage = false;

  bool no_invalid_option = true;
  int c;
  int option_index = 0;
  while (true) {
    c = getopt_long(argc, argv, optstring, long_options, &option_index);
    if (c == -1) {
      break;
    }

    switch (c) {
    case 't':
      // -t, --tiarra
      options.log_format = irclog2json::Options::LogFormat::Tiarra;
      break;
    case 'm':
      // -m, --madoka
      options.log_format = irclog2json::Options::LogFormat::Madoka;
      break;
    case 'j':
      // -j, --iso-2022-jp
      options.iso_2022_jp = true;
      break;
    case 'p':
      // -p, --pretty
      options.pretty = true;
      break;
    case 'h':
      // -h, --help
      print_usage = true;
      break;
    case '?':
      // 無効なオプション
      no_invalid_option = false;
      print_usage = true;
      break;
    default:
      break;
    }
  }

  return no_invalid_option;
}

static picojson::value
ConvertLogToJsonObjects(std::ifstream& ifs, std::string const& channel,
                        struct tm const& tm_date,
                        irclog2json::Options const& options) {
  using irclog2json::message::LineConverter;
  using irclog2json::message::MadokaIso2022JpLineConverter;
  using irclog2json::message::MadokaLineConverter;
  using irclog2json::message::TiarraIso2022JpLineConverter;
  using irclog2json::message::TiarraLineConverter;
  using irclog2json::message::UTF8LineConverter;
  using LogFormat = irclog2json::Options::LogFormat;

  std::unique_ptr<LineConverter> line_converter;

  switch (options.log_format) {
  case LogFormat::Tiarra: {
    auto utf8_line_converter =
        std::make_unique<TiarraLineConverter>(channel, tm_date);

    if (options.iso_2022_jp) {
      line_converter = std::make_unique<TiarraIso2022JpLineConverter>(
          std::move(utf8_line_converter));
    } else {
      line_converter = std::move(utf8_line_converter);
    }

    break;
  }
  case LogFormat::Madoka: {
    auto utf8_line_converter =
        std::make_unique<MadokaLineConverter>(channel, tm_date);

    if (options.iso_2022_jp) {
      line_converter = std::make_unique<MadokaIso2022JpLineConverter>(
          std::move(utf8_line_converter));
    } else {
      line_converter = std::move(utf8_line_converter);
    }

    break;
  }
  default:
    return picojson::value{picojson::array{}};
  }

  irclog2json::Converter converter{&ifs, std::move(line_converter)};
  return converter.Convert();
}

static void PrintUsage(char* argv0, std::ostream& os) {
  os << "Usage: " << argv0 << " (-t | -m) [-j] [-p] LOG_FILE CHANNEL"
     << std::endl;
  os << "  -t, --tiarra          specify that the log file is Tiarra format"
     << std::endl;
  os << "  -m, --madoka          specify that the log file is madoka format"
     << std::endl;
  os << "  -j, --iso-2022-jp     specify that the charset is ISO-2022-JP"
     << std::endl;
  os << "  -p, --pretty          output pretty-printed JSON" << std::endl;
  os << "  -h, --help            display this help and exit" << std::endl;
}
