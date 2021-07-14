/*******************************************************************************
PACKAGE:  Robot
FILE:     Slogbuf.h
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef SLOGBUF_H
#define SLOGBUF_H

/* INCLUDE ********************************************************************/
/* CLASSES ********************************************************************/
class Slog;

class Slogbuf: public std::basic_streambuf<char>
{
public:
  explicit Slogbuf(const std::string& name, slogtypes::type type):
  std::basic_streambuf<char>()
  {
    openlog(name.size() ? name.data() : nullptr, LOG_PID, type);
  }
  ~Slogbuf() override { closelog(); }

protected:
  int_type overflow(int_type c = traits_type::eof()) override
  {
    if(traits_type::eq_int_type(c, traits_type::eof()))
      sync();
    else buffer += traits_type::to_char_type(c);
    
    return c;
  }

  int sync() override
  {
    if(buffer.size())
    {
      syslog(level, "%s", buffer.data());
      
      buffer.clear();
      level = ini_level;
    }
    return 0;
  }

  friend class Slog;
  void set_level(slogtypes::level new_level) noexcept { level = new_level; };

private:
  static constexpr slogtypes::level ini_level = slogtypes::info;
  slogtypes::level level = ini_level;

  std::string buffer;
};

/* GLOBALS ********************************************************************/

#endif /* SLOGBUF_H */
