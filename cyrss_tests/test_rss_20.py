from __future__ import unicode_literals, print_function
import unittest
import os
from .common import load_test_data
from cyrss.feed_parser import parse_feed, CyRssException


class TestRss20(unittest.TestCase):
    def test_sanity_1(self):
        data = load_test_data('jezebel_rss_20.xml')
        feed = parse_feed(data)
        self.assertTrue(u"Celebrity, Sex, Fashion" in feed.description)

    def test_sanity_2(self):
        data = load_test_data('nyt_us_rss_20.xml')
        feed = parse_feed(data)
        self.assertEqual(u"NYT > U.S.", feed.title)

    def test_parse_feed_editor(self):
        doc = """
            <rss version="2.0">
            <channel>
            <managingEditor>an editor</managingEditor>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("an editor", feed.managing_editor)
        self.assertEqual("an editor", feed.managingEditor)

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
                </item>
            </channel>
            </rss>
        """
        feed = parse_feed(doc)
        self.assertEqual("Joe", feed.items[0].author)
        self.assertEqual("Something", feed.items[0].title)
        self.assertEqual("http://www.nothing.com/something", feed.items[0].link)

