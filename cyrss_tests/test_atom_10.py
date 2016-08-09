from __future__ import unicode_literals, print_function
import unittest
import os
from .common import load_test_data
from cyrss.feed_parser import parse_feed, CyRssException

class TestAtom10Sanity(unittest.TestCase):
    def test_sanity(self):
        data = load_test_data('register_uk_headlines_atom.xml')
        feed = parse_feed(data)
        self.assertEqual(u"The Register", feed.title)
        self.assertTrue(u"Biting the hand that feeds" in feed.description)
        entry = feed.items[0]
        expected = 'http://go.theregister.com/feed/www.theregister.co.uk/2016/08/05/pegglecrew_we_hacked_fosshub_so_ransomware_scum_couldnt_and_also_for_fun/'
        self.assertEqual(expected, entry.link)


class TestAtom10Feed(unittest.TestCase):
    def test_title(self):
        doc = """
            <feed xmlns="http://www.w3.org/2005/Atom">
              <title>An Atom Feed</title>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("An Atom Feed", feed.title)

    @unittest.skip
    def test_title_relative_link(self):
        # todo: this is a test from feedparser.
        # https://github.com/kurtmckee/feedparser/blob/develop/tests/wellformed/atom10/relative_uri.xml
        doc = """
          <feed xmlns="http://www.w3.org/2005/Atom">
            <title type="xhtml" xml:base="http://baz.com/x/">
              <div xmlns="http://www.w3.org/1999/xhtml">
                Example <a href="test.html">test</a>
              </div>
            </title>
          </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("http://baz.com/x/test.html", feed.)


class TestAtom10Entry(unittest.TestCase):
    def test_author_1(self):
        doc = """
            <feed xmlns="http://www.w3.org/2005/Atom">
            <entry>
              <author>
                <name>Example author</name>
                <email>me@example.com</email>
                <uri>http://example.com/</uri>
              </author>
            </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("Example author", feed.author)

    def test_id_1(self):
        doc = """
            <feed xmlns="http://www.w3.org/2005/Atom">
              <entry>
                <id>http://example.com</id>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        self.assertEqual("http://example.com", feed.items[0].id)

    def test_entry_link_title(self):
        doc = """
            <feed xmlns="http://www.w3.org/2005/Atom">
              <entry>
                <title>Something</title>
                <link rel="alternate" type="text/html" href="http://www.example.com/"/>
              </entry>
            </feed>
        """
        feed = parse_feed(doc)
        entry = feed.items[0]
        self.assertEqual("http://example.com", entry.link)
        self.assertEqual("Something", entry.title)

