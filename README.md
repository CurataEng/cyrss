# cyrss

`cyrss` is a Python library for fast RSS/Atom parsing.
The underlying library is in c++, with the bindings written in Cython.
It handles byte or unicode strings in both Python 2 and Python 3.


## performance
In an [unscientific benchmark](./bench.py), `cyrss` hovers around 100x the speed of the popular `feedparser` library.
In this particular benchmark, `feedparser` takes around 3.4 seconds to parse 100 feeds.  `cyrss` does the same in around 0.03 seconds.

## dependencies
The c++ component of cyrss has no external dependencies other than a c++11-capable compiler.
The Python library has an additional dependency on `cchardet` for character encoding detection.  Installing the library does not require Cython to be installed, but building the release tarball does.

## licensing
cyrss contains code from two MIT-licensed open source projects, `newsbeuter` and `pugixml`.  Curata is releasing it under.... *LICENSE (fixme)*

## encoding
cyrss uses utf-8 strings internally, but accepts any of the `unicode`/`str`/`bytes` types from Python 2 or Python 3.  `bytes` types can be in any encoding: `cchardet` is used to detect and convert non-utf8 encodings prior to parsing.

## atom/rss versions
cyrss detects and handles the various versions of RSS and Atom.  The resulting `CyFeed` and `CyFeedItem` objects have a normalized set of properties from the two standards (see the API section).


## API
cyrss has three main classes: `CyFeed`, `CyFeedItem` and `CyFeedParser`.

### CyFeed
`CyFeed` has the following properties:
* `title` [unicode]
* `description` [unicode]
* `link` [unicode]
* `items` [Python list of `CyFeedItem` instances]

### CyFeedItem
`CyFeedItem` has the following properties:
* `title` [unicode]
* `description` [unicode]
* `link` [unicode]
* `author` [unicode]
* `author_email` [unicode]
* `pubDate` or `pub_date` [unicode]
* `guid` [unicode]
* `enclosure_url` [unicode]
* `content_encoded` [unicode]
* `itunes_summary` [unicode]

### CyFeedParser
`CyFeedParser` has several methods, each for parsing a different input:

#### `parse`: `(object) -> CyFeed`
Accepts a python object, assumed to be either a unicode or byte string.
Calls either the `parse_unicode` or `parse_bytes` method, depending on the argument's type.  Raises `TypeError` if the argument is not a Python string type.

#### `parse_bytes`: `(bytes) -> CyFeed` (Python 3)
#### `parse_bytes`: `(str) -> CyFeed` (Python 2)
Performs character encoding detection, creates a utf8-encoded cop of the byte string if necessary, and then parses into a `CyFeed` instances.

#### `parse_unicode`: `(str) -> CyFeed` (Python 3)
#### `parse_unicode`: `(unicode) -> CyFeed` (Python 2)
Encodes a unicode string into a utf8 byte string and then parses it into a `CyFeed` instance.  Charset detection is not performed.

#### `parse_utf8_bytes`: `(bytes) -> CyFeed` (Python 3)
#### `parse_utf8_bytes`: `(str) -> CyFeed` (Python 2)
Like `parse_bytes`, but assumes that the input is utf8-encoded and does not attempt character encoding detection.  This is more efficient, but you should only use it if you know what you're doing.


