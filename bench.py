from __future__ import print_function
import time
import pyximport
pyximport.install()
from cyrss.feed_parser import CyFeedParser
from cyrss.feed_parser import time_calls
import feedparser
from cyrss_tests.common import in_data_dir


with open(in_data_dir('jezebel_rss_20.xml')) as f:
    JEZEBEL = f.read()

def cy_parse():
    parser = CyFeedParser()
    parser.parse(JEZEBEL)

def py_parse():
    feedparser.parse(JEZEBEL)


if __name__ == '__main__':
    cy_result = time_calls(cy_parse, 100)
    print('cy : %3.12f' % cy_result)
    py_result = time_calls(py_parse, 100)
    print('py : %3.12f' % py_result)
