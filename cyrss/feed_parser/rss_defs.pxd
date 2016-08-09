from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "rss/Feed.h" namespace "curata::rss":
    cdef struct FeedLink:
        string url

    cdef struct FeedAuthor:
        string name
        string email
        FeedLink link

    cdef struct FeedDateTime:
        string timestamp

    cdef struct FeedGuid:
        string id

    cdef struct FeedEntryGuid:
        FeedGuid guid

    cdef struct FeedEnclosure:
        FeedLink link
        string dtype

    cdef struct FeedEncoding:
        string dtype

    cdef struct FeedLanguage:
        string name

    cdef struct FeedEditor:
        string name

    cdef struct FeedData:
        string data

    cdef cppclass FeedItem:
        FeedEntryGuid guid
        FeedData title
        FeedData description
        FeedData summary
        FeedData itunesSummary
        FeedLink link
        FeedAuthor author
        FeedDateTime pubDate
        FeedEnclosure enclosure
        FeedEncoding encoded
        string base
        vector[string] labels
        vector[string] categories

    cdef cppclass FeedMetadata:
        FeedGuid guid
        FeedData title
        FeedData description
        FeedLink link
        FeedLanguage language
        FeedEncoding encoded
        FeedAuthor author
        FeedEditor managingEditor
        FeedDateTime pubDate

    cdef cppclass Feed:
        FeedMetadata metadata
        vector[FeedItem] items


cdef extern from "rss/parsers/FeedParser.h" namespace "curata::rss::parsers":
    cdef cppclass FeedParser:
        Feed parseFeed(const string& xmlText) except+
