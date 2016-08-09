#include "rss/Feed.h"
#include <string>

/*
 * This isn't a functional test exactly.
 * The goal is just to take the library through its basic steps.
 * Running this under valgrind will report on any memory leaks
 * or related issues.
*/


string loadTestDoc(const std::string &fpath) {
  string testDataPath = "./test_data/jezebel_rss_20.xml";
  return curata::rss::util::loadFile(testDataPath);
}