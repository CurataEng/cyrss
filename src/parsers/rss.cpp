/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


#include "rss/parsers/rss.h"
#include "rss/Feed.h"
#include "rss/parsers/pugi_util.h"
#include "rss/parsers/time_util.h"
#include "rss/exceptions.h"

using namespace std;

namespace curata { namespace rss { namespace parsers {

using pugi_util::getNodeContent;
using pugi_util::getNodeAttr;


FeedFormat getRssType(const std::string& versionStr) {
  if (versionStr.empty()) {
    return FeedFormat::UNKNOWN;
  }
  if (versionStr[0] == '0') {
    if (versionStr.size() < 4) {
      return FeedFormat::UNKNOWN;
    }
    if (versionStr == "0.91") {
      return FeedFormat::RSS_0_91;
    } else if (versionStr == "0.92") {
      return FeedFormat::RSS_0_92;
    } else if (versionStr == "0.94") {
      return FeedFormat::RSS_0_94;
    }
  } else if (versionStr == "2.0" || versionStr == "2") {
    return FeedFormat::RSS_2_0;
  } else if (versionStr == "1.0") {
    return FeedFormat::RSS_1_0;
  }
  return FeedFormat::UNKNOWN;
}

void parseRss09xFeed(Feed& feed, const pugi::xml_node &rootNode) {
  auto channel = rootNode.child("channel");
  if (!channel) {
    throw ParseError("No RSS channel found.");
  }
  string name;
  for (auto node: channel.children()) {
    name = node.name();
    if (name == "title") {
      feed.metadata.title = FeedData::fromXmlNode(node);
    } else if (name == "link") {
      feed.metadata.link.url = getNodeContent(node);
    } else if (name == "language") {
      feed.metadata.language.name = getNodeContent(node);
    } else if (name == "description") {
      feed.metadata.description = FeedData::fromXmlNode(node);
    } else if (name == "managingEditor") {
      feed.metadata.managingEditor.name = getNodeContent(node);
    } else if (name == "pubDate") {
      feed.metadata.pubDate = FeedDateTime::fromRfc822(node);
    } else if (name == "item") {
      feed.items.push_back(parseRss09xFeedItem(node));
    }
  }
}

FeedItem parseRss09xFeedItem(const pugi::xml_node &itemNode) {
  FeedItem feedItem;
  string name;
  for (auto child: itemNode.children()) {
    name = child.name();
    if (name == "title") {
      feedItem.title = FeedData::fromXmlNode(child);
    } else if (name == "dc:title" && feedItem.title.empty()) {
      feedItem.title = FeedData::fromXmlNode(child);
    } else if (name == "dc:description" && feedItem.description.empty()) {
      feedItem.description = FeedData::fromXmlNode(child);
    } else if (name == "link") {
      getNodeContent(feedItem.link.url, child);
    } else if (name == "description") {
      feedItem.description = FeedData::fromXmlNode(child);
    } else if (name == "encoded") {
      getNodeContent(feedItem.encoded.dtype, child);
    } else if (name == "summary") {
      feedItem.summary = FeedData::fromXmlNode(child);
    } else if (name == "guid" || name == "id") {
      getNodeContent(feedItem.guid.guid.id, child);
      feedItem.guid.isPermaLink = true;
      if (pugi_util::getNodeAttr(child, "isPermaLink") == "false") {
        feedItem.guid.isPermaLink = false;
      }
    } else if (name == "pubDate") {
      feedItem.pubDate = FeedDateTime::fromRfc822(child);
    } else if (name == "date" && feedItem.pubDate.empty()) {
      feedItem.pubDate = FeedDateTime::fromW3cDtf(child);
    } else if (name == "author") {
      getNodeContent(feedItem.author.name, child);
    } else if (name == "dc:author" && !feedItem.author.name.size()) {
      getNodeContent(feedItem.author.name, child);
    } else if (name == "creator" && !feedItem.author.name.size()) {
      getNodeContent(feedItem.author.name, child);
    } else if (name == "enclosure") {
      pugi_util::getNodeAttr(feedItem.enclosure.link.url, child, "url");
      pugi_util::getNodeAttr(feedItem.enclosure.dtype, child, "type");
    } else if (name == "group") {
      auto contentChild = child.child("content");
      if (contentChild) {
        pugi_util::getNodeAttr(feedItem.enclosure.link.url, contentChild, "url");
        pugi_util::getNodeAttr(feedItem.enclosure.dtype, contentChild, "type");
      }
    }
  }
  return feedItem;
}



void parseRss10Feed(Feed& feed, const pugi::xml_node &rootNode) {
  auto channel = rootNode.child("channel");
  if (!channel) {
    throw ParseError("No RSS channel found.");
  }
  string name;
  for (auto node: channel.children()) {
    name = node.name();
    if (name == "title") {
      feed.metadata.title = FeedData::fromXmlNode(node);
    } else if (name == "link") {
      feed.metadata.link.url = getNodeContent(node);
    } else if (name == "description") {
      feed.metadata.description = FeedData::fromXmlNode(node);
    } else if (name == "date") {
      feed.metadata.pubDate = FeedDateTime::fromW3cDtf(node);
    } else if (name == "creator") {
      getNodeContent(feed.metadata.author.name, node);
    } else if (name == "item") {
      feed.items.push_back(parseRss10FeedItem(node));
    }
  }
}

FeedItem parseRss10FeedItem(const pugi::xml_node &itemNode) {
  FeedItem feedItem;
  getNodeAttr(feedItem.guid.guid.id, itemNode, "about");
  string name;
  for (auto child: itemNode.children()) {
    name = child.name();
    if (name == "title") {
      feedItem.title = FeedData::fromXmlNode(child);
    } else if (name == "link") {
      getNodeContent(feedItem.link.url, child);
    } else if (name == "description") {
      feedItem.description = FeedData::fromXmlNode(child);
    } else if (name == "date") {
      feedItem.pubDate = FeedDateTime::fromW3cDtf(child);
    } else if (name == "encoded") {
      getNodeContent(feedItem.encoded.dtype, child);
    } else if (name == "summary") {
      feedItem.summary = FeedData::fromXmlNode(child);
    } else if (name == "creator") {
      getNodeContent(feedItem.author.name, child);
    }
  }
  return feedItem;
}

void parseRss20Feed(Feed& feed, const pugi::xml_node &rootNode) {
  parseRss09xFeed(feed, rootNode);
}

}}} // curata::rss::parsers