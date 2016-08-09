/* Adapted from NewsBeuter https://github.com/akrennmair/newsbeuter/blob/master/rss/atom_parser.cpp
 * Original license:
 *   rsspp - Copyright (C) 2008-2012 Andreas Krennmair <ak@newsbeuter.org>
 *   Licensed under the MIT/X Consortium License. See file LICENSE
 * Modifications (C) 2016 Curata Inc.
 *
*/


#include "rss/parsers/rss20.h"
#include "rss/parsers/rss09x.h"

namespace curata { namespace rss { namespace parsers {

void parseRss20Feed(Feed& feed, const pugi::xml_node &rootNode) {
  parseRss09xFeed(feed, rootNode);
}

}}} // curata::rss::parsers