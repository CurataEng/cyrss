/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


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


void parseAtomFeed(Feed &feed, const pugi::xml_node &rootNode) {
  getNodeAttr(feed.language, rootNode, "lang");
  string globalBase = getNodeAttr(rootNode, "base");

  string name;
  for (auto node: rootNode.children()) {
    name = node.name();
    if (name == "title") {
      getNodeContent(feed.title, node);
      getNodeAttr(feed.title_type, node, "type");
      if (feed.title_type == "") {
        feed.title_type = "text";
      }
    } else if (name == "subtitle") {
      feed.description = getNodeContent(node);
    } else if (name == "link") {
      string rel = getNodeAttr(node, "rel");
      if (rel == "alternate") {
        feed.link = url_util::make_absolute_url(globalBase, getNodeAttr(node, "href"));
      }
    } else if (name == "updated") {
      feed.pubDate = time_util::w3cdtf_to_rfc822(getNodeContent(node));
    } else if (name == "entry") {
      feed.items.push_back(parseAtomFeedItem(node, globalBase));
    }
  }
}


FeedItem parseAtomFeedItem(const pugi::xml_node &entryNode, const std::string &defaultGlobalBase) {
    FeedItem feedItem;
    std::string summary;
    std::string summary_type;
    std::string updated;

    std::string base = getNodeAttr(entryNode, "base");
    if (base == "") {
        base = defaultGlobalBase;
    }
    string name;
    for (auto node: entryNode.children()) {
      name = node.name();
      if (name == "author") {
        getNodeContent(feedItem.author, node);
      } else if (name == "title") {
        getNodeContent(feedItem.title, node);
        getNodeAttr(feedItem.title_type, node, "type");
        if (feedItem.title_type == "") {
          feedItem.title_type = "text";
        }
      } else if (name == "content") {
        getNodeContent(feedItem.description, node);
        getNodeAttr(feedItem.description_type, node, "type");
        if (feedItem.description_type == "") {
          feedItem.description_type = "text";
        }
      } else if (name == "id") {
        getNodeContent(feedItem.guid, node);
        feedItem.guid_isPermaLink = false;
      } else if (name == "published") {
        string pubDate = getNodeContent(node);
        pubDate = time_util::w3cdtf_to_rfc822(pubDate);
        feedItem.pubDate = pubDate;
      } else if (name == "updated") {
        updated = time_util::w3cdtf_to_rfc822(getNodeContent(node));
      } else if (name == "link") {
        string rel = getNodeAttr(node, "rel");
        if (rel == "" || rel == "alternate") {
          feedItem.link = url_util::make_absolute_url(base, getNodeAttr(node, "href"));
        } else if (rel == "enclosure") {
          getNodeAttr(feedItem.enclosure_url, node, "href");
          getNodeAttr(feedItem.enclosure_type, node, "type");
        }
      } else if (name == "summary") {
        summary_type = getNodeAttr(node, "type");
        summary = getNodeContent(node);
        if (summary_type == "") {
          summary_type = "text";
        }
      } else if (name == "category") {
        if (getNodeAttr(node, "scheme") == "http://www.google.com/reader/") {
          feedItem.labels.push_back(getNodeAttr(node, "label"));
        }
      }
    }

    if (feedItem.description == "") {
        feedItem.description = summary;
        feedItem.description_type = summary_type;
    }

    if (feedItem.pubDate == "") {
        feedItem.pubDate = updated;
    }

    return feedItem;
}


}}} // curata::rss::parsers