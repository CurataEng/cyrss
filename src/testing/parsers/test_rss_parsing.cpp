#include <map>
#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include "rss/Feed.h"
#include "rss/parsers/rss.h"


using namespace std;
using namespace pugi;
using curata::rss::parsers::getRssType;
using curata::rss::FeedFormat;

TEST(TestRssParsing, TestGetRssType) {
  EXPECT_EQ(FeedFormat::RSS_0_91, getRssType("0.91"));
  EXPECT_EQ(FeedFormat::RSS_0_92, getRssType("0.92"));
  EXPECT_EQ(FeedFormat::RSS_0_94, getRssType("0.94"));
  EXPECT_EQ(FeedFormat::RSS_1_0, getRssType("1.0"));
  EXPECT_EQ(FeedFormat::RSS_2_0, getRssType("2.0"));
  EXPECT_EQ(FeedFormat::RSS_2_0, getRssType("2"));
  EXPECT_EQ(FeedFormat::UNKNOWN, getRssType("nothing"));
  EXPECT_EQ(FeedFormat::UNKNOWN, getRssType(""));
}
