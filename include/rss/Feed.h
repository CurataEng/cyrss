/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/
#pragma once
#include <string>
#include <vector>

namespace curata { namespace rss {

enum class FeedType {
  UNKNOWN = 0,
  RSS_0_91,
  RSS_0_92,
  RSS_1_0,
  RSS_2_0,
  ATOM_0_3,
  ATOM_1_0,
  RSS_0_94,
  ATOM_0_3_NONS
};

struct FeedItem {
  std::string title;
  std::string title_type;
  std::string link;
  std::string description;
  std::string description_type;
  std::string author;
  std::string author_email;

  std::string pubDate;
  std::string guid;
  bool guid_isPermaLink;

  std::string enclosure_url;
  std::string enclosure_type;

  // extensions:
  std::string content_encoded;
  std::string itunes_summary;

  // Atom-specific:
  std::string base;
  std::vector<std::string> labels;
  std::vector<std::string> categories;
};

struct Feed {
  std::string encoding;

  FeedType rss_version;
  std::string title;
  std::string title_type;
  std::string description;
  std::string link;
  std::string language;
  std::string managingEditor;
  std::string dc_creator;
  std::string pubDate;

  std::vector<FeedItem> items;
};


}} // curata::rss
