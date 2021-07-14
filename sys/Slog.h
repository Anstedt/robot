#ifndef SLOG_H
#define SLOG_H

#include <ostream>
#include <streambuf>
#include <string>

#include "SlogTypes.h"
#include "Slogbuf.h"

class Slog: public std::basic_ostream<char>
{
public:
  explicit Slog(const std::string& name = std::string(), log::type type = log::user):
  std::basic_ostream<char>(&streambuf),
      streambuf(name, type)
  { }

  Slog& operator<<(log::level level) noexcept
  {
    streambuf.set_level(level);
    return (*this);
  }

private:
    Slogbuf streambuf;
};

static Slog slog;

#define SLOG (slog << __FILE__ << ":" << __LINE__ << " ")

#endif // SLOG_H


