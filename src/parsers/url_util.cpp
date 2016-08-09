#include "rss/parsers/url_util.h"
#include <string>
#include <sstream>

using namespace std;

namespace curata { namespace rss { namespace parsers { namespace url_util {

string make_absolute_url(const string &baseUrl, const string &fullUrl) {
  if (fullUrl.find("http") == 0) {
    return fullUrl;
  }
  if (!baseUrl.size()) {
    return fullUrl;
  }
  ostringstream result;
  result << baseUrl << fullUrl;
  return result.str();
}

}}}} // curata::rss::parsers::url_util
