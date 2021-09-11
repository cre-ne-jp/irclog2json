#define _XOPEN_SOURCE

#include <filesystem>
#include <optional>
#include <string>

#include <ctime>

#include "file_name_manager.h"

namespace irclog2json {

FileNameManager::FileNameManager(const std::string& input_path_str)
    : input_path_{input_path_str} {
}

FileNameManager::~FileNameManager() = default;

std::optional<struct tm> FileNameManager::GetDate() const {
  struct tm tm_date {};
  std::string filename = input_path_.filename().string();

  char* rest = strptime(filename.c_str(), "%Y%m%d.txt", &tm_date);
  if (rest == nullptr || *rest != '\0') {
    return std::nullopt;
  }

  return tm_date;
}

std::string FileNameManager::GetOutputPath() const {
  return std::filesystem::path{input_path_}.replace_extension(".json").string();
}

} // namespace irclog2json
