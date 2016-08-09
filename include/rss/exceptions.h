#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include "rss/util.h"

namespace curata { namespace rss {

class RssException: public std::runtime_error {
 public:
  RssException(const std::string& msg);

  template<typename T>
  RssException(const T& ref): RssException(util::toString<T>(ref)){}

};

class ParseError: public RssException {
 public:
  ParseError(const std::string& msg);

  template<typename T>
  ParseError(const T& ref): RssException(ref){}
};



}} // curata::rss