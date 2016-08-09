import os
from distutils.core import setup, Extension

OPTIMIZED = '-O2'
UNOPTIMIZED = '-O0'
OPTIMIZATION = OPTIMIZED


def root_rel(fpath):
    here = os.path.dirname(os.path.realpath(__file__))
    return os.path.join(here, fpath)


EXTRA_SRC = (
    'parsers/atom.cpp',
    'parsers/FeedParser.cpp',
    'parsers/rss09x.cpp',
    'parsers/rss10.cpp',
    'parsers/rss20.cpp',
    'parsers/pugi_util.cpp',
    'parsers/time_util.cpp',
    'parsers/url_util.cpp',
    'xml/pugixml.cpp',
    'util.cpp',
    'exceptions.cpp'
)

CY_SRC = (
    'cyrss/feed_parser/feed_parser.cpp',
    'cyrss/feed_parser/compat.cpp',
    'cyrss/feed_parser/benchmarker.cpp'
)

def get_extension():
    extra_src = map(lambda f: os.path.join('src', f), EXTRA_SRC)
    extra_src = list(map(root_rel, extra_src))
    all_src = list(CY_SRC) + extra_src
    return Extension('cyrss.feed_parser.feed_parser',
        sources=all_src,
        language='c++',
        include_dirs=[root_rel('include')],
        extra_compile_args=['--std=c++11', OPTIMIZATION]
    )


setup(
    name='cyrss',
    version='0.9.0',
    description="A fast RSS parser",
    license='MIT',
    package_data={
        'cyrss': ['*.py', '*.pxd', '*.hpp', '*.h', '*.cpp', '*.c'],
        'cyrss.feed_parser': ['*.py', '*.pxd', '*.hpp', '*.h', '*.cpp', '*.c']
    },
    packages=[
        'cyrss', 'cyrss.feed_parser'
    ],
    install_requires=['cchardet>=1.0.0'],
    ext_modules=[get_extension()],
    provides=['cyrss']
)
