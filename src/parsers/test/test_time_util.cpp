#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "rss/parsers/time_util.h"
using namespace std;
namespace time_util = curata::rss::parsers::time_util;

TEST(TestTimeUtil, TestConvertW3CDTF) {
  const string timeStr = "2003-02-15T13:50:05-05:00";
  auto result = time_util::w3cdtf_to_rfc822(timeStr);
  EXPECT_EQ("Sat, 15 Feb 2003 19:50:05 +0000", result);
}
