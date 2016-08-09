from __future__ import unicode_literals, print_function
import unittest
import os
from .common import load_test_data
from cyrss.feed_parser import parse_feed, CyRssException

class TestRDFChannel(unittest.TestCase):
    def test_title(self):
        doc = """
          <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns="http://purl.org/rss/1.0/">
            <channel rdf:about="http://example.com/index.rdf">
              <title>Example feed</title>
            </channel>
          </rdf:RDF>
        """
        feed = parse_feed(doc)
        self.assertEqual("Example feed", feed.title)
