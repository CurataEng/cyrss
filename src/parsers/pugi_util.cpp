#include "rss/parsers/pugi_util.h"
#include <string>

using std::string;
using pugi::xml_node;

namespace curata { namespace rss { namespace parsers { namespace pugi_util {


string getNodeContent(const xml_node& node) {
  string result;
  getNodeContent(result, node);
  return result;
}

void getNodeContent(string& result, const xml_node& node) {
  result = node.child_value();
}

string getNodeAttr(const xml_node& node, const string& attrName) {
  string result {""};
  getNodeAttr(result, node, attrName);
  return result;
}

void getNodeAttr(string& result, const xml_node &node, const string& attrName) {
  result = "";
  for (auto attr: node.attributes()) {
    if (attr.name() == attrName) {
      result = attr.value();
      break;
    }
  }
}

}}}} // curata::rss::parsers::pugi_util
