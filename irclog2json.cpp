#include "converter.h"

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <libgen.h>

void PrintUsage(char* argv0);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    PrintUsage(argv[0]);
    std::exit(1);
  }

  std::string filename(argv[1]);
  std::string channel(argv[2]);

  char* file_basename = basename(argv[1]);

  struct tm tm_date;
  char* rest = strptime(file_basename, "%Y%m%d.txt", &tm_date);
  if (rest == NULL || *rest != '\0') {
    std::cerr << filename << ": Invalid date" << std::endl;
    std::exit(1);
  }

  std::string out_filename(filename);
  out_filename.replace(out_filename.size() - 3, 3, "json");

  std::ifstream ifs;

  ifs.open(argv[1]);
  if (ifs.fail()) {
    std::perror(argv[1]);
    std::exit(1);
  }

  irclog2json::Converter converter(ifs, channel, tm_date);
  picojson::value result = converter.Convert();

  ifs.close();

  std::ofstream ofs;

  ofs.open(out_filename);
  if (ofs.fail()) {
    std::perror(argv[1]);
    std::exit(1);
  }

  ofs << result.serialize(true);

  ofs.close();

  std::cout << "Output " << out_filename << '.' << std::endl;

  return 0;
}

void PrintUsage(char* argv0) {
  std::cerr << "Usage: " << argv0 << " TIARRA_LOG_FILE CHANNEL" << std::endl;
}
