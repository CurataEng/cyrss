/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/

#pragma once

#include "rss/Feed.h"
#include "rss/xml/pugixml.hpp"

namespace curata { namespace rss { namespace parsers {

class FeedParser  {
 public:
  Feed parseFeed(const pugi::xml_document &rootNode);
  Feed parseFeed(const std::string& doc);
};

}}} // curata::rss::parsers
