#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "rss/parsers/pugi_util.h"
using namespace std;
using namespace pugi;
namespace pugi_util = curata::rss::parsers::pugi_util;

string getDocument() {
  vector<string> lines {
    "<?xml version=\"1.0\"/?>",
    "<something>fish</something>",
    "<grandparent age=\"old\">",
    "  <parent name=\"joe\">",
    "    <child toy=\"car\">CAR_TEXT</child>",
    "  </parent>",
    "</grandparent>"
  };
  ostringstream oss;
  for (auto &line: lines) {
    oss << line;
  }
  return oss.str();
}

TEST(TestPugiUtil, TestGetNodeContent) {
  pugi::xml_document doc;
  auto docStr = getDocument();
  auto status = doc.load_string(docStr.c_str());
  EXPECT_TRUE(status);
  auto something = doc.child("something");
  string expected = "fish";
  EXPECT_EQ("fish", pugi_util::getNodeContent(something));
}

TEST(TestPugiUtil, TestGetNodeAttr) {
  pugi::xml_document doc;
  auto docStr = getDocument();
  auto status = doc.load_string(docStr.c_str());
  EXPECT_TRUE(status);
  auto parent = doc.child("grandparent").child("parent");
  EXPECT_EQ("joe", pugi_util::getNodeAttr(parent, "name"));
}
