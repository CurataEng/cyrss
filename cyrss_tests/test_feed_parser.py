from __future__ import unicode_literals, print_function
import unittest
import os
from .common import load_test_data
from cyrss.feed_parser import parse_feed, CyRssException


class BadInputTests(unittest.TestCase):
    def test_invalid_xml(self):
        with self.assertRaises(CyRssException):
            parse_feed("not xml")

    def test_empty_string(self):
        with self.assertRaises(CyRssException):
            parse_feed("")

    def test_wrong_type(self):
        with self.assertRaises(CyRssException):
            parse_feed(None)

        with self.assertRaises(TypeError):
            parse_feed(None)


class TestReprs(unittest.TestCase):
    def test_feed_repr(self):
        jezebel = load_test_data('jezebel_rss_20.xml')
        self.assertTrue("Jezebel" in repr(jezebel))

class TestAttrGetters(unittest.TestCase):
    def test_dtypes(self):
        jezebel = parse_feed(load_test_data('jezebel_rss_20.xml'))
        first_entry = jezebel.items[0]
        self.assertEqual(u"text", first_entry.title_type)
        self.assertEqual(u"text", first_entry.description_type)
        self.assertEqual(u"", first_entry.summary_type)
        self.assertEqual(u"", first_entry.enclosure_type)
