#pragma once
#include <string>

namespace curata { namespace rss { namespace parsers { namespace time_util {

// stolen from newsbeuter
std::string w3cdtf_to_rfc822(const std::string& w3cdtf);


}}}} // curata::rss::parsers::time_util
