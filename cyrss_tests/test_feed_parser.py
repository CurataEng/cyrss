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


class TestFeedParser(unittest.TestCase):

    def test_parsing_atom10(self):
        data = load_test_data('register_uk_headlines_atom.xml')
        feed = parse_feed(data)
        self.assertEqual(u"The Register", feed.title)
        self.assertTrue(u"Biting the hand that feeds" in feed.description)

    def test_parsing_atom_10_more(self):
        data = load_test_data('register_uk_headlines_atom.xml')
        feed = parse_feed(data)
        entry = feed.items[0]
        expected = 'http://go.theregister.com/feed/www.theregister.co.uk/2016/08/05/pegglecrew_we_hacked_fosshub_so_ransomware_scum_couldnt_and_also_for_fun/'
        self.assertEqual(expected, entry.link)

