#pragma once

#include "rss/xml/pugixml.hpp"
#include <string>

namespace curata { namespace rss { namespace parsers { namespace pugi_util {

std::string getNodeContent(const pugi::xml_node&);
void getNodeContent(std::string&, const pugi::xml_node&);

std::string getNodeAttr(const pugi::xml_node&, const std::string& attr);
void getNodeAttr(std::string&, const pugi::xml_node&, const std::string& attr);

}}}} // curata::rss::parsers::pugi_util
