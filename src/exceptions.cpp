#include "rss/exceptions.h"

namespace curata { namespace rss {

RssException::RssException(const std::string &msg)
  : std::runtime_error(msg) {}


ParseError::ParseError(const std::string &msg)
  : RssException(msg) {}

}} // curata::rss
