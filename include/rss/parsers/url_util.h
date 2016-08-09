#pragma once

#include "rss/xml/pugixml.hpp"
#include <string>

namespace curata { namespace rss { namespace parsers { namespace url_util {

std::string make_absolute_url(const std::string&, const std::string&);

}}}} // curata::rss::parsers::url_util