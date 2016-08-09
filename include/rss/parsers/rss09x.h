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

void parseRss09xFeed(Feed& f, const pugi::xml_node&);
FeedItem parseRss09xFeedItem(const pugi::xml_node&);


}}} // curata::rss::parsers

