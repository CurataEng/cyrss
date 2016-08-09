from rss_defs cimport (
    FeedParser,
    Feed,
    FeedItem,
    FeedAuthor,
    FeedMetadata,
    FeedData,
    FeedDateTime,
    FeedLink,
    FeedEditor,
    FeedEncoding,
    FeedEnclosure,
    FeedLanguage,
    FeedGuid,
    FeedEntryGuid
)

from libcpp.vector cimport vector
import cchardet
import sys

PY3_MODE = True

if sys.version_info[0] == 2:
    PY3_MODE = False

if PY3_MODE:
    str_type = str
    bytes_type = bytes
else:
    str_type = unicode
    bytes_type = str


__all__ = ('CyFeed', 'CyFeedItem', 'CyFeedParser', 'parse_feed', 'CyRssException')

class CyRssException(RuntimeError):
    pass

class CyRssTypeError(CyRssException, TypeError):
    pass

cdef class CyFeedItem:
    cdef FeedItem feed_item_

    def __cinit__(self, FeedItem cpp_feed_item):
        self.feed_item_ = cpp_feed_item

    property title:
        def __get__(self):
            return self.feed_item_.title.data

    property title_type:
        def __get__(self):
            return "text"

    property description:
        def __get__(self):
            return self.feed_item_.description.data

    property description_type:
        def __get__(self):
            return "text"

    property link:
        def __get__(self):
            return self.feed_item_.link.url

    property author:
        def __get__(self):
            return self.feed_item_.author.name

    property author_email:
        def __get__(self):
            return self.feed_item_.author.email

    property pubDate:
        def __get__(self):
            return self.feed_item_.pubDate

    property pub_date:
        def __get__(self):
            return self.feed_item_.pubDate

    property guid:
        def __get__(self):
            return self.feed_item_.guid.guid.id

    property enclosure_url:
        def __get__(self):
            return self.enclosure.link.url

    property enclosure_type:
        def __get__(self):
            return "text"


cdef class CyFeedMetadata:
    cdef FeedMetadata metadata_

    def __cinit__(self, FeedMetadata cpp_metadata):
        self.metadata_ = cpp_metadata

    property title:
        def __get__(self):
            return self.metadata_.title.data

    property description:
        def __get__(self):
            return self.metadata_.description.data

    property link:
        def __get__(self):
            return self.metadata_.link.url

    property managing_editor:
        def __get__(self):
            return self.metadata_.managingEditor.name

    property author:
        def __get__(self):
            return self.metadata_.author.name

    property author_email:
        def __get__(self):
            return self.metadata_.author.email


cdef class CyFeed:
    cdef CyFeedMetadata metadata_
    cdef object items_

    def __cinit__(self, Feed cpp_feed):
        self.metadata_ = CyFeedMetadata(cpp_feed.metadata)
        self.items = []
        cdef CyFeedItem current_item
        for item in cpp_feed.items:
            current_item = CyFeedItem(item)
            self.items_.append(current_item)

    property title:
        def __get__(self):
            return self.metadata_.title

    property description:
        def __get__(self):
            return self.metadata_.description

    property link:
        def __get__(self):
            return self.metadata_.link

    property language:
        def __get__(self):
            return self.metadata_.language

    property pub_date:
        def __get__(self):
            return self.metadata_.pub_date

    property pubDate:
        def __get__(self):
            return self.metadata_.pub_date

    property managing_editor:
        def __get__(self):
            return self.metadata_.managing_editor

    property managingEditor:
        def __get__(self):
            return self.metadata_.managing_editor

    property items:
        def __get__(self):
            return self.items_


cdef class CyFeedParser:
    cdef FeedParser parser_

    cpdef CyFeed parse_utf8_bytes(self, bytes feed_xml):
        cdef Feed cpp_feed
        cdef CyFeed feed
        try:
            cpp_feed = self.parser_.parseFeed(feed_xml)
            feed = CyFeed(cpp_feed)
            return feed
        except RuntimeError as err:
            raise CyRssException(err)

    cpdef CyFeed parse_bytes(self, bytes feed_xml):
        cdef bytes as_utf8 = feed_xml
        encoding = cchardet.detect(feed_xml)['encoding']
        if encoding != 'UTF-8':
            as_utf8 = feed_xml.decode(encoding).encode('utf-8')
        return self.parse_utf8_bytes(as_utf8)

    cpdef CyFeed parse_unicode(self, unicode feed_xml):
        return self.parse_utf8_bytes(feed_xml.encode('utf-8'))

    cpdef CyFeed parse(self, object feed_xml):
        if isinstance(feed_xml, str_type):
            return self.parse_unicode(feed_xml)
        elif isinstance(feed_xml, bytes_type):
            return self.parse_bytes(feed_xml)
        else:
            raise CyRssTypeError("expected unicode or bytes type")


cpdef CyFeed parse_feed(object feed_xml):
    cdef CyFeedParser feed_parser = CyFeedParser()
    return feed_parser.parse(feed_xml)
