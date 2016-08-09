/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


#include "rss/parsers/FeedParser.h"
#include "rss/parsers/rss20.h"
#include "rss/parsers/rss10.h"
#include "rss/parsers/rss09x.h"
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

FeedType getRssType(const std::string& versionStr) {
  if (versionStr.empty()) {
    return FeedType::UNKNOWN;
  }
  if (versionStr[0] == '0') {
    if (versionStr.size() < 4) {
      return FeedType::UNKNOWN;
    }
    if (versionStr == "0.91") {
      return FeedType::RSS_0_91;
    } else if (versionStr == "0.92") {
      return FeedType::RSS_0_92;
    } else if (versionStr == "0.94") {
      return FeedType::RSS_0_94;
    }
  } else if (versionStr == "2.0" || versionStr == "2") {
    return FeedType::RSS_2_0;
  } else if (versionStr == "1.0") {
    return FeedType::RSS_1_0;
  }
  return FeedType::UNKNOWN;
}

Feed FeedParser::parseFeed(const pugi::xml_document &doc) {
  Feed result;
  result.rss_version = FeedType::UNKNOWN;
  auto root = doc.root();
  auto baseElem = root.child("rss");
  if (baseElem) {
    auto versionStr = pugi_util::getNodeAttr(baseElem, "version");
    auto rssType = getRssType(versionStr);
    if (rssType == FeedType::UNKNOWN) {
      throw ParseError("unknown RSS version.");
    }
    result.rss_version = rssType;
  }
  if (!baseElem) {
    baseElem = root.child("RDF");
    if (baseElem) {
      result.rss_version = FeedType::RSS_1_0;
    }
  }
  if (!baseElem) {
    baseElem = root.child("feed");
    if (baseElem) {
      auto feedNamespace = pugi_util::getNodeAttr(baseElem, "xmlns");
      if (feedNamespace == "http://www.w3.org/2005/Atom") {
        result.rss_version = FeedType::ATOM_1_0;
      } else if (feedNamespace == "http://purl.org/atom/ns#") {
        result.rss_version = FeedType::ATOM_0_3;
      } else {
        auto versionStr = pugi_util::getNodeAttr(baseElem, "version");
        if (versionStr == "0.3") {
          result.rss_version = FeedType::ATOM_0_3_NONS;
        }
      }

      if (result.rss_version == FeedType::UNKNOWN) {
        throw ParseError("Seems to be an atom feed, but couldn't determine version.");
      }
    }
  }
  if (!baseElem) {
    throw ParseError("Unknown feed type");
  }
  switch(result.rss_version) {
    case FeedType::RSS_1_0:
      parseRss10Feed(result, baseElem);
      break;
    case FeedType::RSS_0_91:
    case FeedType::RSS_0_92:
    case FeedType::RSS_0_94:
      parseRss09xFeed(result, baseElem);
      break;
    case FeedType::RSS_2_0:
      parseRss20Feed(result, baseElem);
      break;
    case FeedType::ATOM_0_3:
    case FeedType::ATOM_1_0:
    case FeedType::ATOM_0_3_NONS:
      parseAtomFeed(result, baseElem);
      break;

    case FeedType::UNKNOWN:
    default:
      throw ParseError("unknown feed type");
      break;
  }
  return result;
}


}}} // curata::rss::parsers