#include <map>
#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "rss/parsers/pugi_util.h"
#include "rss/parsers/FeedParser.h"
#include "rss/util.h"
#include "rss/Feed.h"

using namespace std;
using namespace pugi;
namespace pugi_util = curata::rss::parsers::pugi_util;
using curata::rss::parsers::FeedParser;
using curata::rss::Feed;
using curata::rss::FeedItem;

namespace {
string getJezebelDocument() {
  string testDataPath = "./test_data/jezebel_rss_20.xml";
  return curata::rss::util::loadFile(testDataPath);
}
string getRegisterDocument() {
  string testDataPath = "./test_data/register_uk_headlines_atom.xml";
  return curata::rss::util::loadFile(testDataPath);
}
}

TEST(TestFeedParser, BasicRssTest) {
  FeedParser fp;
  auto doc = getJezebelDocument();
  auto parsed = fp.parseFeed(doc);
  EXPECT_TRUE(parsed.title.find("Jezebel") != string::npos);
  EXPECT_TRUE(parsed.description.find("Celebrity, Sex, Fashion") != string::npos);
  map<string, FeedItem*> itemsByTitle;
  for (auto &item: parsed.items) {
    itemsByTitle.insert(std::make_pair(item.title, &item));
  }
  EXPECT_TRUE(itemsByTitle.size() > 10);
  string bestTitle = "Last Night's Real Housewives of New York Was a Carnival Cruise of Bad Romances";
  auto itemPtr = itemsByTitle[bestTitle];
  EXPECT_TRUE(itemPtr->title.find("Last") == 0);
}

TEST(TestFeedParser, BasicAtomTest) {
  FeedParser fp;
  auto doc = getRegisterDocument();
  auto parsed = fp.parseFeed(doc);
  EXPECT_EQ("The Register", parsed.title);
}
