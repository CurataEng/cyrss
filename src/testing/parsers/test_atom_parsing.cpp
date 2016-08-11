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

struct AuthorParseTestContext {
  pugi::xml_document pugiDoc;
  std::string rawDoc;
  AuthorParseTestContext(const vector<string> &docParts) {
    rawDoc = util::joinWith("", docParts);
    pugiDoc.load_string(rawDoc.c_str());
  }
  FeedAuthor run() {
    auto authorNode = pugiDoc.root().child("author");
    FeedAuthor author;
    parsers::parseAtomAuthor(author, authorNode);
    return author;
  }
};

TEST(TestAtomParsing, TestParseAuthorSimple) {
  vector<string> atomDoc {
    "<author>",
    "  <name>Joe</name>",
    "  <email>joe@joe.com</email>",
    "  <uri>http://eoj.org</uri>",
    "</author>"
  };
  AuthorParseTestContext ctx(atomDoc);
  auto author = ctx.run();
  EXPECT_EQ("Joe", author.name);
  EXPECT_EQ("joe@joe.com", author.email);
  EXPECT_EQ("http://eoj.org", author.link.url);
}

TEST(TestAtomParsing, TestParseAuthorMissingSome) {
  vector<string> atomDoc {
    "<author>",
    "  <email>joe@joe.com</email>",
    "</author>"
  };
  AuthorParseTestContext ctx(atomDoc);
  auto author = ctx.run();
  EXPECT_EQ("", author.name);
  EXPECT_EQ("joe@joe.com", author.email);
  EXPECT_EQ("", author.link.url);
}

TEST(TestAtomParsing, TestParseAuthorExtraElements) {
  vector<string> atomDoc {
    "<author>",
    "  <email>joe@joe.com</email>",
    "  <extraneous>x</extraneous>",
    "</author>"
  };
  AuthorParseTestContext ctx(atomDoc);
  auto author = ctx.run();
  EXPECT_EQ("", author.name);
  EXPECT_EQ("joe@joe.com", author.email);
  EXPECT_EQ("", author.link.url);
}

TEST(TestAtomParsing, TestParseAuthorNonStandardLink) {
  vector<string> atomDoc {
    "<author>",
    "  <name>Joe</name>",
    "  <email>joe@joe.com</email>",
    "  <link>http://eoj.org</link>",
    "</author>"
  };
  AuthorParseTestContext ctx(atomDoc);
  auto author = ctx.run();
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
  AuthorParseTestContext ctx(atomDoc);
  bool thrown = false;
  try {
    ctx.run();
  } catch (const ParseError &err) {
    thrown = true;
  }
  EXPECT_TRUE(thrown);
}
