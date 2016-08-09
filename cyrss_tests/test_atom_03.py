from __future__ import unicode_literals, print_function
import unittest
import os
from .common import load_test_data
from cyrss.feed_parser import parse_feed, CyRssException


class TestAtom03Feed(unittest.TestCase):
    def test_feed_author(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <author>
                <name>An Author</name>
                <email>tim@gmail.com</email>
                <link>http://itstim.com</link>
              </author>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("An Author", feed.author)

    def test_feed_id(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <id>http://something.com</id>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("http://something.com", feed.id)

    def test_feed_link_rel(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <link rel="alternate" type="text/html" href="http://something.com"/>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("http://something.com", feed.link)

    def test_feed_title_1(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <title type="text/plain">Atom Title</title>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("Atom Title", feed.title)

    def test_feed_title_2(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <title>Atom Title</title>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("Atom Title", feed.title)

    def test_feed_modified_date(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <modified>Mon, 08 Aug 2016 13:07:26 GMT</modified>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("Mon, 08 Aug 2016 13:07:26 GMT", feed.pub_date)
        self.assertEqual("Mon, 08 Aug 2016 13:07:26 GMT", feed.pubDate)

class TestAtom03Entry(unittest.TestCase):
    def test_summary_1(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <entry>
                <summary>abc</summary>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("abc", feed.items[0].description)

    def test_timestamp_created(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <entry>
                <created>Mon, 08 Aug 2016 13:07:26 GMT</created>
              </entry>
            </feed>
        """

        feed = parse_feed(doc)
        self.assertEqual(
            "Mon, 08 Aug 2016 13:07:26 GMT",
            feed.items[0].pub_date
        )

    def test_timestamp_updated(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <entry>
                <published>2016-08-08T13:07:26Z</published>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual(
            "Mon, 08 Aug 2016 14:07:26 +0000",
            feed.items[0].pub_date
        )

    def test_timestamp_issued(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <entry>
                <issued>Mon, 08 Aug 2016 13:07:26 GMT</issued>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual(
            "Mon, 08 Aug 2016 13:07:26 GMT",
            feed.items[0].pub_date
        )

    def test_entry_author_1(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
                <entry>
                    <author>
                        <name>Joe</name>
                        <email>joe@joe.joe</email>
                        <url>http://myworld.joe</url>
                    </author>
                </entry>
            </feed>
        """

        feed = parse_feed(doc)
        self.assertEqual(
            "Joe",
            feed.items[0].author
        )

    def test_entry_content_1(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <entry>
                <content>some content</content>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual(
            "some content",
            feed.items[0].content
        )

    def test_media_thumbnail(self):
        doc = """
            <feed version="0.3" xmlns="http://purl.org/atom/ns#">
              <entry>
                <media:thumbnail xmlns:media="http://search.yahoo.com/mrss/"
                  url="http://example.com/thumbnail.jpg" height="72" width="72" />
                <title>Example Atom</title>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("http://example.com/thumbnail.jpg", feed.items[0].media_thumbnail)
