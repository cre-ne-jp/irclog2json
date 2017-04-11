#include "options.h"
#include "converter.h"

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <clocale>
#include <libgen.h>
#include <getopt.h>

/**
 * @brief コマンドライン引数からオプションを解析する。
 * @param [in] argc コマンドライン引数の個数。
 * @param [in] argv コマンドライン引数の配列。
 * @param [out] options オプションの設定先。
 * @param [out] print_usage 使用法を出力するか。
 * @retval <true> 有効なオプションのみ含まれたいた場合。
 * @retval <false> 無効なオプションが含まれていた場合。
 */
static bool ParseOptions(
  int argc, char* argv[], irclog2json::Options& options, bool& print_usage
);
/**
 * @brief 使用法を表示する。
 * @param argv0 コマンドライン引数の最初に設定されるプログラム名。
 * @param os 出力ストリーム。
 */;
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

  irclog2json::Converter converter(ifs, channel, tm_date);
  picojson::value result = converter.Convert();

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

static bool ParseOptions(
  int argc, char* argv[], irclog2json::Options& options, bool& print_usage
) {
  constexpr struct option long_options[] = {
    {"tiarra", no_argument, nullptr, 't'},
    {"pretty", no_argument, nullptr, 'p'},
    {"help", no_argument, nullptr, 'h'},
    {0, 0, 0, 0}
  };
  const char* optstring = "tph";

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

static void PrintUsage(char* argv0, std::ostream& os) {
  os << "Usage: " << argv0 << " -t [-p] LOG_FILE CHANNEL" << std::endl;
  os << "  -t, --tiarra      specify that the log file is Tiarra format" << std::endl;
  os << "  -p, --pretty      output pretty-printed JSON" << std::endl;
  os << "  -h, --help        display this help and exit" << std::endl;
}