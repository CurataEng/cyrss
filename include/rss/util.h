#pragma once
#include <sstream>
#include <string>

namespace curata { namespace rss { namespace util {

template<typename T>
std::string toString(const T& ref) {
  std::ostringstream oss;
  oss << ref;
  return oss.str();
}

std::string loadFile(const std::string &fpath);

}}} // curata::rss::util
