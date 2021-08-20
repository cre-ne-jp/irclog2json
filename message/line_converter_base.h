#pragma once

#include <string>
#include <regex>
#include <memory>
#include <ctime>

namespace irclog2json {
  namespace message {
    class MessageBase;

    class LineConverterBase {
    public:
      static std::string RemoveControlCodes(std::string const& s);

      LineConverterBase(std::string const& channel, struct tm const& tm_date);

      virtual ~LineConverterBase();

      LineConverterBase(LineConverterBase const&) = default;
      LineConverterBase(LineConverterBase&&) = default;
      LineConverterBase& operator =(LineConverterBase const&) = delete;
      LineConverterBase& operator =(LineConverterBase&&) = delete;

      std::unique_ptr<MessageBase> ToMessage(std::string const& line) const;

    protected:
      const std::string channel_;
      const struct tm tm_date_;

      void SetHMS(struct tm& timestamp, std::smatch const& m) const;

    private:
      virtual std::unique_ptr<MessageBase> DoToMessage(std::string const& line) const = 0;
    };
  }
}
