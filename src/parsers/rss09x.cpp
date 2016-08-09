/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


#include "rss/parsers/rss09x.h"
#include "rss/parsers/pugi_util.h"
#include "rss/parsers/time_util.h"
#include "rss/exceptions.h"
using namespace std;


namespace curata { namespace rss { namespace parsers {

using pugi_util::getNodeContent;
using pugi_util::getNodeAttr;


void parseRss09xFeed(Feed& feed, const pugi::xml_node &rootNode) {
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
    } else if (name == "language") {
      feed.language = getNodeContent(node);
    } else if (name == "description") {
      feed.description = getNodeContent(node);
    } else if (name == "managingEditor") {
      feed.managingEditor = getNodeContent(node);
    } else if (name == "item") {
      feed.items.push_back(parseRss09xFeedItem(node));
    }
  }
}

FeedItem parseRss09xFeedItem(const pugi::xml_node &itemNode) {
  FeedItem feedItem;
  std::string author;
  std::string dc_date;
  string name;
  for (auto child: itemNode.children()) {
    name = child.name();
    if (name == "title") {
      getNodeContent(feedItem.title, child);
    } else if (name == "link") {
      getNodeContent(feedItem.link, child);
    } else if (name == "description") {
      getNodeContent(feedItem.description, child);
    } else if (name == "encoded") {
      getNodeContent(feedItem.content_encoded, child);
    } else if (name == "summary") {
      getNodeContent(feedItem.itunes_summary, child);
    } else if (name == "guid") {
      getNodeContent(feedItem.guid, child);
      feedItem.guid_isPermaLink = true;
      if (pugi_util::getNodeAttr(child, "isPermaLink") == "false") {
        feedItem.guid_isPermaLink = false;
      }
    } else if (name == "pubDate") {
      getNodeContent(feedItem.pubDate, child);
    } else if (name == "date") {
      dc_date = time_util::w3cdtf_to_rfc822(getNodeContent(child));
    } else if (name == "author") {
      getNodeContent(feedItem.author, child);
      getNodeContent(feedItem.author, child);
    } else if (name == "creator") {
      author = getNodeContent(child);
    } else if (name == "enclosure") {
      pugi_util::getNodeAttr(feedItem.enclosure_url, child, "url");
      pugi_util::getNodeAttr(feedItem.enclosure_type, child, "type");
    } else if (name == "group") {
      auto contentChild = child.child("content");
      if (contentChild) {
        pugi_util::getNodeAttr(feedItem.enclosure_url, contentChild, "url");
        pugi_util::getNodeAttr(feedItem.enclosure_type, contentChild, "type");
      }
    }
  }
  if (feedItem.author == "") {
    feedItem.author = author;
  }
  if (feedItem.pubDate == "") {
    feedItem.pubDate = dc_date;
  }
  return feedItem;
}


}}} // curata::rss::parsers