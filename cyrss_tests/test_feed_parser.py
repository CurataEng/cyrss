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
