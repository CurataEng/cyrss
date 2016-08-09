from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "rss/Feed.h" namespace "curata::rss":
    cdef struct FeedItem:
        string title
        string title_type
        string link
        string description
        string description_type
        string author
        string author_email
        string pubDate
        string guid
        string enclosure_url
        string enclosure_type
        string content_encoded
        string itunes_summary
        string base
        vector[string] labels
        vector[string] categories

    cdef struct Feed:
        string encoding
        string title
        string title_type
        string description
        string link
        string language
        string managingEditor
        string dc_creator
        string pubDate
        vector[FeedItem] items


cdef extern from "rss/parsers/FeedParser.h" namespace "curata::rss::parsers":
    cdef cppclass FeedParser:
        Feed parseFeed(const string& xmlText) except+
