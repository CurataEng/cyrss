/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/

#include "rss/Feed.h"
#include "rss/parsers/atom.h"
#include "rss/parsers/pugi_util.h"
#include "rss/parsers/time_util.h"
#include "rss/parsers/url_util.h"

#include "rss/exceptions.h"
#include <cstring>

using pugi::xml_document;
using pugi::xml_node;
using namespace std;

namespace curata { namespace rss { namespace parsers {

using pugi_util::getNodeContent;
using pugi_util::getNodeAttr;


void parseAtomAuthor(FeedAuthor& author, const pugi::xml_node &authorNode) {
  string nodeName = authorNode.name();
  if (nodeName != "author") {
    throw ParseError("expected author node.");
  }
  bool haveDetailedAuthor = false;
  for (auto child: authorNode.children()) {
    haveDetailedAuthor = true;
    nodeName = child.name();
    if (nodeName == "name") {
      getNodeContent(author.name, child);
    } else if (nodeName == "email") {
      getNodeContent(author.email, child);
    } else if (nodeName == "uri") {
      getNodeContent(author.link.url, child);
    } else if (nodeName == "link") {
      if (author.link.url.empty()) {
        getNodeContent(author.link.url, child);
      }
    }
  }
  if (!haveDetailedAuthor) {
    // the node is just a bare <author>NAME</author> element.
    getNodeContent(author.name, authorNode);
  }
}

void parseAtomFeed(Feed &feed, const pugi::xml_node &rootNode) {
  getNodeAttr(feed.metadata.language.name, rootNode, "lang");
  string globalBase = getNodeAttr(rootNode, "base");

  string name;
  for (auto node: rootNode.children()) {
    name = node.name();
    if (name == "title") {
      feed.metadata.title = std::move(FeedData::fromXmlNode(node));
    } else if (name == "id") {
      getNodeContent(feed.metadata.guid.id, node);
    } else if (name == "author") {
      parseAtomAuthor(feed.metadata.author, node);
    } else if (name == "subtitle") {
      feed.metadata.description = std::move(FeedData::fromXmlNode(node));
    } else if (name == "link") {
      string rel = getNodeAttr(node, "rel");
      if (rel == "alternate") {
        feed.metadata.link.url = url_util::make_absolute_url(globalBase, getNodeAttr(node, "href"));
      }
    } else if (name == "updated") {
      feed.metadata.pubDate = FeedDateTime::fromW3cDtf(node);
    } else if (name == "modified" || name == "created" || name == "issued") {
      if (feed.metadata.pubDate.empty()) {
        feed.metadata.pubDate = FeedDateTime::fromRfc822(node);
      }
    } else if (name == "entry") {
      feed.items.push_back(parseAtomFeedItem(node, globalBase));
    }
  }
}


FeedItem parseAtomFeedItem(const pugi::xml_node &entryNode, const std::string &defaultGlobalBase) {
    FeedItem feedItem;

    std::string base = getNodeAttr(entryNode, "base");
    if (base == "") {
        base = defaultGlobalBase;
    }
    string name;
    for (auto node: entryNode.children()) {
      name = node.name();
      if (name == "author") {
        parseAtomAuthor(feedItem.author, node);
      } else if (name == "title") {
        feedItem.title = FeedData::fromXmlNode(node);
      } else if (name == "content") {
        feedItem.description = FeedData::fromXmlNode(node);
      } else if (name == "id") {
        getNodeContent(feedItem.guid.guid.id, node);
        feedItem.guid.isPermaLink = false;
      } else if (name == "published") {
        feedItem.pubDate = FeedDateTime::fromW3cDtf(node);
      } else if (name == "updated" || name == "created" || name == "issued" || name == "modified") {
        if (feedItem.pubDate.empty()) {
          feedItem.pubDate = FeedDateTime::fromRfc822(node);
        }
      } else if (name == "link") {
        string rel = getNodeAttr(node, "rel");
        if (rel == "" || rel == "alternate") {
          feedItem.link.url = url_util::make_absolute_url(base, getNodeAttr(node, "href"));
        } else if (rel == "enclosure") {
          getNodeAttr(feedItem.enclosure.link.url, node, "href");
          getNodeAttr(feedItem.enclosure.link.dtype, node, "type");
        }
      } else if (name == "summary") {
        feedItem.summary = FeedData::fromXmlNode(node);
      } else if (name == "category") {
        if (getNodeAttr(node, "scheme") == "http://www.google.com/reader/") {
          feedItem.labels.push_back(getNodeAttr(node, "label"));
        }
      }
    }

    return feedItem;
}


}}} // curata::rss::parsers