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

    property title:
        def __get__(self):
            return self.feed_item_.title.data.decode('utf-8')

    property title_type:
        def __get__(self):
            return self.feed_item_.title.dtype.decode('utf-8')

    property description:
        def __get__(self):
            return self.feed_item_.description.data.decode('utf-8')

    property description_type:
        def __get__(self):
            return self.feed_item_.description.dtype.decode('utf-8')

    property summary:
        def __get__(self):
            return self.feed_item_.summary.data.decode('utf-8')

    property summary_type:
        def __get__(self):
            return self.feed_item_.summary.dtype.decode('utf-8')

    property link:
        def __get__(self):
            return self.feed_item_.link.url.decode('utf-8')

    property author:
        def __get__(self):
            return self.feed_item_.author.name.decode('utf-8')

    property author_email:
        def __get__(self):
            return self.feed_item_.author.email.decode('utf-8')

    property pubDate:
        def __get__(self):
            return self.pub_date

    property pub_date:
        def __get__(self):
            return self.feed_item_.pubDate.timestamp.decode('utf-8')

    property guid:
        def __get__(self):
            return self.feed_item_.guid.guid.id.decode('utf-8')

    property id:
        def __get__(self):
            return self.guid

    property enclosure_url:
        def __get__(self):
            return self.feed_item_.enclosure.link.url.decode('utf-8')

    property enclosure_type:
        def __get__(self):
            return self.feed_item_.enclosure.dtype.decode('utf-8')

    property content:
        def __get__(self):
            if self.description:
                return self.description
            return self.summary


cdef class CyFeedMetadata:
    cdef FeedMetadata metadata_

    property title:
        def __get__(self):
            return self.metadata_.title.data.decode('utf-8')

    property description:
        def __get__(self):
            return self.metadata_.description.data.decode('utf-8')

    property link:
        def __get__(self):
            return self.metadata_.link.url.decode('utf-8')

    property managing_editor:
        def __get__(self):
            return self.metadata_.managingEditor.name.decode('utf-8')

    property author:
        def __get__(self):
            return self.metadata_.author.name.decode('utf-8')

    property author_email:
        def __get__(self):
            return self.metadata_.author.email.decode('utf-8')

    property pub_date:
        def __get__(self):
            return self.metadata_.pubDate.timestamp.decode('utf-8')

    property pubDate:
        def __get__(self):
            return self.metadata_.pubDate.timestamp.decode('utf-8')


cdef class CyFeed:
    cdef CyFeedMetadata metadata_
    cdef object items_

    def __init__(self):
        self.metadata_ = CyFeedMetadata()
        self.items_ = []

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

    property author:
        def __get__(self):
            return self.metadata_.author

    property author_email:
        def __get__(self):
            return self.metadata_.author_email

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
        cdef CyFeed feed = CyFeed()
        try:
            cpp_feed = self.parser_.parseFeed(feed_xml)
            feed.metadata_.metadata_ = cpp_feed.metadata
            for item in cpp_feed.items:
                current_item = CyFeedItem()
                current_item.feed_item_ = item
                feed.items_.append(current_item)

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
