#include <map>
#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include "rss/Feed.h"
#include "rss/exceptions.h"
#include "rss/parsers/atom.h"
#include "rss/util.h"

using namespace std;
using namespace curata::rss;
using curata::rss::parsers::parseAtomAuthor;
using curata::rss::FeedAuthor;

TEST(TestAtomParsing, TestParseAuthorSimple) {
  vector<string> atomDoc {
    "<author>",
    "  <name>Joe</name>",
    "  <email>joe@joe.com</email>",
    "  <uri>http://eoj.org</uri>",
    "</author>"
  };
  auto xmlStr = util::joinWith("", atomDoc);
  pugi::xml_document pugiDoc;
  pugiDoc.load_string(xmlStr.c_str());
  auto root = pugiDoc.root();
  auto authorNode = root.child("author");
  FeedAuthor author;
  parsers::parseAtomAuthor(author, authorNode);
  EXPECT_EQ("Joe", author.name);
  EXPECT_EQ("joe@joe.com", author.email);
  EXPECT_EQ("http://eoj.org", author.link.url);
}

TEST(TestAtomParsing, TestParseAuthorNonStandardLink) {
  vector<string> atomDoc {
    "<author>",
    "  <name>Joe</name>",
    "  <email>joe@joe.com</email>",
    "  <link>http://eoj.org</link>",
    "</author>"
  };
  auto xmlStr = util::joinWith("", atomDoc);
  pugi::xml_document pugiDoc;
  pugiDoc.load_string(xmlStr.c_str());
  auto root = pugiDoc.root();
  auto authorNode = root.child("author");
  FeedAuthor author;
  parsers::parseAtomAuthor(author, authorNode);
  EXPECT_EQ("Joe", author.name);
  EXPECT_EQ("joe@joe.com", author.email);
  EXPECT_EQ("http://eoj.org", author.link.url);
}

TEST(TestAtomParsing, TestParseAuthorWrongRootNode) {
  vector<string> atomDoc {
    "<bad>",
    "  <name>Joe</name>",
    "  <email>joe@joe.com</email>",
    "  <uri>http://eoj.org</uri>",
    "</bad>"
  };
  auto xmlStr = util::joinWith("", atomDoc);
  pugi::xml_document pugiDoc;
  pugiDoc.load_string(xmlStr.c_str());
  auto root = pugiDoc.root();
  FeedAuthor author;

  bool thrown = false;
  try {
    parsers::parseAtomAuthor(author, root);
  } catch (const ParseError &err) {
    thrown = true;
  }
  EXPECT_TRUE(thrown);
}
