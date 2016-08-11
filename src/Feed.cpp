#include "rss/Feed.h"
#include "rss/parsers/time_util.h"
#include "rss/parsers/pugi_util.h"

namespace time_util = curata::rss::parsers::time_util;
namespace pugi_util = curata::rss::parsers::pugi_util;

namespace curata { namespace rss {

FeedDataType feedDataTypeOfString(const std::string &typeStr) {
  if (typeStr == "text") {
    return FeedDataType::TEXT;
  } else if (typeStr == "html" || typeStr == "text/html") {
    return FeedDataType::HTML;
  }
  return FeedDataType::UNKNOWN;
}

std::string stringOfFeedDataType(FeedDataType dtype) {
  switch(dtype) {
    case FeedDataType::TEXT:
      return "text";
    case FeedDataType::HTML:
      return "html";
    case FeedDataType::UNKNOWN:
    default:
      return "";
  }
  return "";
}

FeedData FeedData::fromXmlNode(const pugi::xml_node &node) {
  FeedData feedData;
  pugi_util::getNodeContent(feedData.data, node);
  feedData.dtype = feedDataTypeOfString(pugi_util::getNodeAttr(node, "type"));
  if (feedData.dtype == FeedDataType::UNKNOWN) {
    feedData.dtype = FeedDataType::TEXT;
  }
  return feedData;
}

std::string FeedData::getTypeString() {
  return stringOfFeedDataType(dtype);
}

bool FeedData::empty() const {
  return data.empty();
}

bool FeedDateTime::empty() const {
  return timestamp.empty();
}

FeedDateTime FeedDateTime::fromW3cDtf(const std::string &timeStr) {
  FeedDateTime feedTime;
  feedTime.timestamp = time_util::w3cdtf_to_rfc822(timeStr);
  return feedTime;
}

FeedDateTime FeedDateTime::fromW3cDtf(const pugi::xml_node &node) {
  return fromW3cDtf(pugi_util::getNodeContent(node));
}

FeedDateTime FeedDateTime::fromRfc822(const std::string &timeStr) {
  FeedDateTime feedTime;
  feedTime.timestamp = timeStr;
  return feedTime;
}

FeedDateTime FeedDateTime::fromRfc822(const pugi::xml_node &node) {
  return fromRfc822(pugi_util::getNodeContent(node));
}

}};
