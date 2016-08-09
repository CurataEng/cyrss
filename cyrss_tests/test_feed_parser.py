from __future__ import unicode_literals, print_function
import unittest
import os
from .common import in_data_dir
from cyrss.feed_parser import parse_feed, CyRssException


def load_test_data(name):
    fpath = in_data_dir(name)
    with open(fpath, 'r') as f:
        rss_data = f.read()
    return rss_data


class TestFeedParser(unittest.TestCase):
    def test_parsing_rss20_1(self):
        data = load_test_data('jezebel_rss_20.xml')
        feed = parse_feed(data)
        self.assertTrue(u"Celebrity, Sex, Fashion" in feed.description)

    def test_parsing_rss20_2(self):
        data = load_test_data('nyt_us_rss_20.xml')
        feed = parse_feed(data)
        self.assertEqual(u"NYT > U.S.", feed.title)

    def test_parsing_atom10(self):
        data = load_test_data('register_uk_headlines_atom.xml')
        feed = parse_feed(data)
        self.assertEqual(u"The Register", feed.title)
        self.assertTrue(u"Biting the hand that feeds" in feed.description)

    def test_invalid_rss(self):
        with self.assertRaises(CyRssException):
            parse_feed("not xml")

    def test_parsing_atom_10_more(self):
        data = load_test_data('register_uk_headlines_atom.xml')
        feed = parse_feed(data)
        entry = feed.items[0]
        expected = 'http://go.theregister.com/feed/www.theregister.co.uk/2016/08/05/pegglecrew_we_hacked_fosshub_so_ransomware_scum_couldnt_and_also_for_fun/'
        self.assertEqual(expected, entry.link)
