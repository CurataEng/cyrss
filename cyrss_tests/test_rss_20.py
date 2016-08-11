from __future__ import unicode_literals, print_function
import unittest
import os
from .common import load_test_data
from cyrss.feed_parser import parse_feed, CyRssException


class TestRss20Sanity(unittest.TestCase):
    def test_sanity_1(self):
        data = load_test_data('jezebel_rss_20.xml')
        feed = parse_feed(data)
        self.assertTrue(u"Celebrity, Sex, Fashion" in feed.description)
        entry1 = feed.items[0]
        self.assertEqual(u"Madeleine Davies", entry1.author)
        expected_start = "<img src=\"https://i.kinja-img.com/gawker-media/image/upload/s--v7hy1Ig---/c_fit,fl_progressive,q_80,w_636/uasglfrbmqr1l2t1hxc1.jpg\" />"
        self.assertTrue(entry1.description.startswith(expected_start))

    def test_sanity_2(self):
        data = load_test_data('nyt_us_rss_20.xml')
        feed = parse_feed(data)
        self.assertEqual(u"NYT > U.S.", feed.title)


class TestRss20Channel(unittest.TestCase):
    def test_parse_channel_1(self):
        doc = """
            <rss version="2.0">
            <channel>
              <managingEditor>an editor</managingEditor>
              <title>A Title</title>
              <link>http://example.com</link>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("an editor", feed.managing_editor)
        self.assertEqual("an editor", feed.managingEditor)
        self.assertEqual("A Title", feed.title)
        self.assertEqual("http://example.com", feed.link)

    def test_no_items(self):
        doc = """"
            <rss version="2.0">
            <channel>
              <title>Something</title>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("Something", feed.title)
        self.assertEqual(0, len(feed.items))


class TestRss20Entry(unittest.TestCase):
    def test_parse_entry_1(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <author>Joe</author>
                    <title>Something</title>
                    <link>http://www.nothing.com/something</link>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("Joe", feed.items[0].author)
        self.assertEqual("Something", feed.items[0].title)
        self.assertEqual("http://www.nothing.com/something", feed.items[0].link)


    def test_parse_entry_2(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <author>Joe</author>
                    <title>Something</title>
                    <link>http://www.nothing.com/something</link>
                    <description>a description</description>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("Joe", feed.items[0].author)
        self.assertEqual("Something", feed.items[0].title)
        self.assertEqual("http://www.nothing.com/something", feed.items[0].link)
        self.assertEqual("a description", feed.items[0].description)

    def test_entry_author_email(self):
        doc = """"
            <rss version="2.0">
            <channel>
                <title>Something</title>
                <item>
                    <author>Sam (sam@gmail.com)</author>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("Sam (sam@gmail.com)", feed.items[0].author)

    def test_parse_entry_dc_author_1(self):
        doc = """
            <rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1">
            <channel>
                <item>
                    <dc:author>A DC Author</dc:author>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("A DC Author", feed.items[0].author)

    def test_parse_entry_dc_title_1(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <dc:title>A DC Title</dc:title>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("A DC Title", feed.items[0].title)

    def test_parse_entry_dc_title_2(self):
        doc = """
            <rss version="2.0" xmln:dc="http://purl.org/dc/elements/1.1">
            <channel>
                <item>
                    <dc:title>A DC Title</dc:title>
                    <title>Non-DC title</title>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("Non-DC title", feed.items[0].title)

    def test_parse_entry_dc_description_1(self):
        doc = """
            <rss version="2.0" xmln:dc="http://purl.org/dc/elements/1.1">
            <channel>
                <item>
                    <dc:description>a dc description</dc:description>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("a dc description", feed.items[0].description)

    def test_parse_entry_dc_description_2(self):
        doc = """
            <rss version="2.0" xmln:dc="http://purl.org/dc/elements/1.1">
            <channel>
                <item>
                    <description>a non-dc description</description>
                    <dc::description>a dc description</dc::description>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("a non-dc description", feed.items[0].description)

    def test_parse_entry_description_summary_1(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <description>a description</description>
                    <summary>a summary</summary>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("a summary", feed.items[0].summary)
        self.assertEqual("a description", feed.items[0].description)

    def test_parse_entry_description_summary_2(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <summary>a summary</summary>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("a summary", feed.items[0].summary)

    def test_parse_entry_enclosure_url(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <enclosure url="http://something.com/bam.png" length="100" type="image/png" />
                    <link>http://something.com</link>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("http://something.com/bam.png", feed.items[0].enclosure_url)

    def test_parse_entry_enclosure_type(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <enclosure url="http://something.com/bam.png" length="100" type="image/png" />
                    <link>http://something.com</link>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("image/png", feed.items[0].enclosure_type)

    def test_parse_entry_guid(self):
        doc = """
            <rss version="2.0">
            <channel>
                <item>
                    <id>some-guid</id>
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("some-guid", feed.items[0].guid)
