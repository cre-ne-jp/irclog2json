#pragma once

#include <filesystem>
#include <optional>
#include <string>

#include <ctime>

namespace irclog2json {

/** ファイル名管理を担うクラス。 */
class FileNameManager {
public:
  FileNameManager(const std::string& input_path_str);
  ~FileNameManager();

  /** ファイル名から日付を取得する。 */
  std::optional<struct tm> GetDate() const;

  /** 出力ファイルのパスを取得する。 */
  std::string GetOutputPath() const;

private:
  /** 入力されたパス。 */
  std::filesystem::path input_path_;
};

} // namespace irclog2json
