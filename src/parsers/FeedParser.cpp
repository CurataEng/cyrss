/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


#include "rss/parsers/FeedParser.h"
#include "rss/parsers/rss.h"
#include "rss/parsers/atom.h"
#include "rss/parsers/pugi_util.h"
#include "rss/exceptions.h"
#include <cstring>

using pugi::xml_document;
using pugi::xml_node;
using namespace std;

namespace curata { namespace rss { namespace parsers {

Feed FeedParser::parseFeed(const string &docString) {
  pugi::xml_document doc;
  auto status = doc.load_string(docString.c_str());
  if (!status) {
    throw ParseError("couldn't parse xml");
  }
  return parseFeed(doc);
}


Feed FeedParser::parseFeed(const pugi::xml_document &doc) {
  Feed result;
  result.metadata.format = FeedFormat::UNKNOWN;
  auto root = doc.root();
  auto baseElem = root.child("rss");
  if (baseElem) {
    auto versionStr = pugi_util::getNodeAttr(baseElem, "version");
    auto format = getRssType(versionStr);
    if (format == FeedFormat::UNKNOWN) {
      throw ParseError("unknown RSS version.");
    }
    result.metadata.format = format;
  }
  if (!baseElem) {
    baseElem = root.child("RDF");
    if (baseElem) {
      result.metadata.format = FeedFormat::RSS_1_0;
    }
  }
  if (!baseElem) {
    baseElem = root.child("feed");
    if (baseElem) {
      auto feedNamespace = pugi_util::getNodeAttr(baseElem, "xmlns");
      if (feedNamespace == "http://www.w3.org/2005/Atom") {
        result.metadata.format = FeedFormat::ATOM_1_0;
      } else if (feedNamespace == "http://purl.org/atom/ns#") {
        result.metadata.format = FeedFormat::ATOM_0_3;
      } else {
        auto versionStr = pugi_util::getNodeAttr(baseElem, "version");
        if (versionStr == "0.3") {
          result.metadata.format = FeedFormat::ATOM_0_3_NONS;
        }
      }
      if (result.metadata.format == FeedFormat::UNKNOWN) {
        throw ParseError("Seems to be an atom feed, but couldn't determine version.");
      }
    }
  }
  if (!baseElem) {
    throw ParseError("Unknown feed format");
  }
  switch(result.metadata.format) {
    case FeedFormat::RSS_1_0:
      parseRss10Feed(result, baseElem);
      break;
    case FeedFormat::RSS_0_91:
    case FeedFormat::RSS_0_92:
    case FeedFormat::RSS_0_94:
      parseRss09xFeed(result, baseElem);
      break;
    case FeedFormat::RSS_2_0:
      parseRss20Feed(result, baseElem);
      break;
    case FeedFormat::ATOM_0_3:
    case FeedFormat::ATOM_1_0:
    case FeedFormat::ATOM_0_3_NONS:
      parseAtomFeed(result, baseElem);
      break;

    case FeedFormat::UNKNOWN:
    default:
      throw ParseError("unknown feed format");
      break;
  }
  return result;
}


}}} // curata::rss::parsers
