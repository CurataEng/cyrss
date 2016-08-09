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

template<typename TCollection>
std::string joinWith(const std::string &joiner, const TCollection &items) {
  if (!items.size()) {
    return "";
  }
  std::ostringstream oss;
  size_t lastIdx = items.size() - 1;
  size_t idx = 0;
  for (auto &item: items) {
    oss << item;
    if (idx != lastIdx) {
      oss << joiner;
    }
  }
  return oss.str();
}

std::string loadFile(const std::string &fpath);

}}} // curata::rss::util
