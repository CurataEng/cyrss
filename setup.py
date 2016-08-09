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

def get_extensions():
    extra_src = map(lambda f: os.path.join('src', f), EXTRA_SRC)
    extra_src = list(map(root_rel, extra_src))
    return [
        Extension('cyrss.feed_parser.feed_parser',
            sources=['cyrss/feed_parser/feed_parser.cpp'] + extra_src,
            language='c++',
            include_dirs=[root_rel('include')],
            extra_compile_args=['--std=c++11', OPTIMIZATION]
        ),
        Extension('cyrss.feed_parser.benchmarker',
            sources=['cyrss/feed_parser/benchmarker.cpp'],
            language='c++'
        ),
        Extension('cyrss.feed_parser.compat',
            sources=['cyrss/feed_parser/compat.cpp'],
            langue='c++'
        )
    ]


setup(
    name='cyrss',
    version='0.9.2',
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
    ext_modules=get_extensions(),
    provides=['cyrss']
)
