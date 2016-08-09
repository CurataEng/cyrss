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
#include "rss/xml/pugixml.hpp"

namespace curata { namespace rss {

enum class FeedFormat {
  UNKNOWN = 0,
  RSS_0_91,
  RSS_0_92,
  RSS_0_94,
  RSS_1_0,
  RSS_2_0,
  ATOM_0_3,
  ATOM_1_0,
  ATOM_0_3_NONS
};

enum class FeedDataType {
  UNKNOWN = 0,
  TEXT = 1,
  HTML = 2
};

FeedDataType feedDataTypeOfString(const std::string&);

struct FeedData {
  FeedDataType dtype {FeedDataType::UNKNOWN};
  std::string data;
  bool empty() const;
  static FeedData fromXmlNode(const pugi::xml_node&);
};

struct FeedLink {
  std::string url;
  std::string dtype;
};

struct FeedAuthor {
  std::string name;
  std::string email;
  FeedLink link;
};

struct FeedDateTime {
  std::string timestamp;
  bool empty() const;
  static FeedDateTime fromW3cDtf(const std::string&);
  static FeedDateTime fromW3cDtf(const pugi::xml_node&);
  static FeedDateTime fromRfc822(const std::string&);
  static FeedDateTime fromRfc822(const pugi::xml_node&);

};

struct FeedGuid {
  std::string id;
};

struct FeedEntryGuid {
  FeedGuid guid;
  bool isPermaLink {false};
};

struct FeedEnclosure {
  FeedLink link;
  std::string dtype;
};

struct FeedEncoding {
  std::string dtype;
};


struct FeedLanguage {
  std::string name;
};


class FeedItem {
 public:
  FeedEntryGuid guid;
  FeedData title;
  FeedData description;
  FeedData summary;
  FeedData itunesSummary;
  FeedLink link;
  FeedAuthor author;
  FeedDateTime pubDate;
  FeedEnclosure enclosure;

  // extensions:
  FeedEncoding encoded;

  // Atom-specific:
  std::string base;
  std::vector<std::string> labels;
  std::vector<std::string> categories;
};

struct FeedEditor {
  std::string name;
};

class FeedMetadata {
 public:
  FeedGuid guid;
  FeedData title;
  FeedData description;
  FeedFormat format;
  FeedLink link;
  FeedLanguage language;
  FeedEncoding encoded;
  FeedAuthor author;
  FeedEditor managingEditor;
  FeedDateTime pubDate;
};

class Feed {
 public:
  FeedMetadata metadata;
  std::vector<FeedItem> items;
};


}} // curata::rss
