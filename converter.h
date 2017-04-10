#include <string>
#include <fstream>
#include <ctime>
#include <picojson.h>

namespace tiarra2json {
  class Converter {
  public:
    Converter(std::ifstream& f, std::string const& channel, struct tm const& tm_date);
    ~Converter() = default;

    Converter(Converter const&) = delete;
    Converter(Converter&&) = delete;
    Converter& operator =(Converter const&) = delete;
    Converter& operator =(Converter&&) = delete;

    picojson::value Convert() const;

  private:
    std::ifstream& f_;
    const std::string channel_;
    const struct tm tm_date_;
  };
}
