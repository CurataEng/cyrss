/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


#include "rss/parsers/rss10.h"
#include "rss/parsers/pugi_util.h"
#include "rss/parsers/time_util.h"
#include "rss/exceptions.h"

using namespace std;

namespace curata { namespace rss { namespace parsers {

using pugi_util::getNodeContent;
using pugi_util::getNodeAttr;

void parseRss10Feed(Feed& feed, const pugi::xml_node &rootNode) {
  auto channel = rootNode.child("channel");
  if (!channel) {
    throw ParseError("No RSS channel found.");
  }
  string name;
  for (auto node: channel.children()) {
    name = node.name();
    if (name == "title") {
      feed.title = getNodeContent(node);
      feed.title_type = "text";
    } else if (name == "link") {
      feed.link = getNodeContent(node);
    } else if (name == "description") {
      feed.description = getNodeContent(node);
    } else if (name == "date") {
      feed.pubDate = time_util::w3cdtf_to_rfc822(getNodeContent(node));
    } else if (name == "creator") {
      feed.dc_creator = getNodeContent(node);
    } else if (name == "item") {
      feed.items.push_back(parseRss10FeedItem(node));
    }
  }
}

FeedItem parseRss10FeedItem(const pugi::xml_node &itemNode) {
  FeedItem feedItem;
  getNodeAttr(feedItem.guid, itemNode, "about");
  string name;
  for (auto child: itemNode.children()) {
    name = child.name();
    if (name == "title") {
      getNodeContent(feedItem.title, child);
    } else if (name == "link") {
      getNodeContent(feedItem.link, child);
    } else if (name == "description") {
      getNodeContent(feedItem.description, child);
    } else if (name == "date") {
      auto dcDate = getNodeContent(child);
      feedItem.pubDate = time_util::w3cdtf_to_rfc822(dcDate);
    } else if (name == "encoded") {
      getNodeContent(feedItem.content_encoded, child);
    } else if (name == "summary") {
      getNodeContent(feedItem.itunes_summary, child);
    } else if (name == "creator") {
      getNodeContent(feedItem.author, child);
    }
  }
  return feedItem;
}


}}} // curata::rss::parsers