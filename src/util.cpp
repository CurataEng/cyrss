#include "rss/util.h"
#include <fstream>

namespace curata { namespace rss { namespace util {

std::string loadFile(const std::string &fpath) {
  std::ifstream aFile;
  std::string line;
  aFile.open(fpath);
  std::ostringstream oss;
  while (std::getline(aFile, line)) {
    oss << line;
  }
  aFile.close();
  return oss.str();
}

}}} // curata::rss::util