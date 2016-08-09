from rss_defs cimport FeedParser, Feed, FeedItem
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

cdef class CyFeedItem:
    cdef unicode title_
    cdef unicode title_type_
    cdef unicode description_
    cdef unicode description_type_
    cdef unicode link_
    cdef unicode author_
    cdef unicode author_email_
    cdef unicode pubDate_
    cdef unicode guid_
    cdef unicode enclosure_url_
    cdef unicode content_encoded_
    cdef unicode itunes_summary_
    cdef unicode base_

    def __init__(self, FeedItem cpp_feed_item):
        self.title_ = cpp_feed_item.title.decode('utf-8')
        self.title_type_ = cpp_feed_item.title_type.decode('utf-8')
        self.description_ = cpp_feed_item.description.decode('utf-8')
        self.description_type_ = cpp_feed_item.description_type.decode('utf-8')
        self.link_ = cpp_feed_item.link.decode('utf-8')
        self.author_ = cpp_feed_item.author.decode('utf-8')
        self.author_email_ = cpp_feed_item.author_email.decode('utf-8')
        self.pubDate_ = cpp_feed_item.pubDate.decode('utf-8')
        self.guid_ = cpp_feed_item.guid.decode('utf-8')
        self.enclosure_url_ = cpp_feed_item.enclosure_url.decode('utf-8')
        self.content_encoded_ = cpp_feed_item.content_encoded.decode('utf-8')
        self.itunes_summary_ = cpp_feed_item.itunes_summary.decode('utf-8')
        self.base_ = cpp_feed_item.base.decode('utf-8')

    property title:
        def __get__(self):
            return self.title_

    property title_type:
        def __get__(self):
            return self.title_type_

    property description:
        def __get__(self):
            return self.description_

    property description_type:
        def __get__(self):
            return self.description_type_

    property link:
        def __get__(self):
            return self.link_

    property author:
        def __get__(self):
            return self.author_

    property author_email:
        def __get__(self):
            return self.author_email_

    property pubDate:
        def __get__(self):
            return self.pubDate_

    property pub_date:
        def __get__(self):
            return self.pubDate_

    property guid:
        def __get__(self):
            return self.guid_

    property enclosure_url_:
        def __get__(self):
            return self.guid_


cdef class CyFeed:
    cdef unicode title_
    cdef unicode description_
    cdef unicode link_
    cdef unicode language_
    cdef unicode managing_editor_
    cdef unicode dc_creator_
    cdef unicode pub_date_
    cdef object items_

    def __init__(self, Feed cpp_feed):
        self.title_ = cpp_feed.title.decode('utf-8')
        self.description_ = cpp_feed.description.decode('utf-8')
        self.link_ = cpp_feed.link.decode('utf-8')
        self.language_ = cpp_feed.language.decode('utf-8')
        self.managing_editor_ = cpp_feed.managingEditor.decode('utf-8')
        self.dc_creator_ = cpp_feed.dc_creator.decode('utf-8')
        self.pub_date_ = cpp_feed.pubDate.decode('utf-8')

        self.items_ = []
        cdef CyFeedItem current_item
        for item in cpp_feed.items:
            current_item = CyFeedItem(item)
            self.items_.append(current_item)

    property title:
        def __get__(self):
            return self.title_

    property description:
        def __get__(self):
            return self.description_

    property link:
        def __get__(self):
            return self.link_

    property language:
        def __get__(self):
            return self.language_

    property pub_date:
        def __get__(self):
            return self.pub_date

    property pubDate:
        def __get__(self):
            return self.pub_date

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
            raise TypeError("expected unicode or bytes type")


cpdef CyFeed parse_feed(object feed_xml):
    cdef CyFeedParser feed_parser = CyFeedParser()
    return feed_parser.parse(feed_xml)
