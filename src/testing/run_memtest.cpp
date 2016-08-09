#include "rss/Feed.h"
#include "rss/util.h"
#include "rss/parsers/FeedParser.h"
#include <string>
#include <iostream>
/*
 * This isn't a functional test exactly.
 * The goal is just to take the library through its basic steps.
 * Running this under valgrind will report on any memory leaks
 * or related issues.
*/

using namespace std;
using curata::rss::parsers::FeedParser;
using curata::rss::Feed;
using curata::rss::util::logInfo;


string loadTestDoc(const std::string &fpath) {
  string testDataPath = "./test_data/" + fpath;
  return curata::rss::util::loadFile(testDataPath);
}

size_t countTitles(Feed &feed) {
  // we're only doing this calculation to make sure
  // the parse output is actually used.  otherwise we run
  // the risk of it being optimized out by the compiler.
  // (though unlikely on -O0)
  size_t titleCounter;
  for (auto &entry: feed.items) {
    titleCounter += entry.title.data.size();
  }
  return titleCounter;
}



// the duplication is here to make valgrind's output easy
// to understand.  don't try to DRY this.

size_t runRss() {
  auto rssDoc = loadTestDoc("jezebel_rss_20.xml");
  FeedParser parser;
  auto parsed = parser.parseFeed(rssDoc);
  return countTitles(parsed);
}

size_t runAtom() {
  auto atomDoc = loadTestDoc("register_uk_headlines_atom.xml");
  FeedParser parser;
  auto parsed = parser.parseFeed(atomDoc);
  return countTitles(parsed);
}



int main() {
  logInfo("running RSS");
  auto rssTitles = runRss();
  logInfo("rss title size: ", rssTitles);
  logInfo("running Atom");
  auto atomTitles = runAtom();
  logInfo("atom title size: ", atomTitles);
  logInfo("end");
}
