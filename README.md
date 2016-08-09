# cyrss

`cyrss` is a Python library for fast RSS/Atom parsing.
The underlying library is in c++, with the bindings written in Cython.
It handles byte or unicode strings in both Python 2 and Python 3.

## Installing
```bash
pip install cyrss
```

## Simple Example
```python
from cyrss import parse_feed
import requests

feed_data = requests.get('http://www.example.com/rss').content
feed = parse_feed(feed_data)
for entry in feed.items:
    pass

```
See the detailed API further down in the readme.


## Performance
In an [unscientific benchmark](./bench.py), `cyrss` hovers around 100x the speed of the popular `feedparser` library.
In this particular benchmark, `feedparser` takes around 3.4 seconds to parse 100 feeds.  `cyrss` does the same in around 0.03 seconds.

## Dependencies
The c++ component of cyrss has no external dependencies other than a c++11-capable compiler.
The Python library has an additional dependency on `cchardet` for character encoding detection.  Installing the library does not require Cython to be installed, but building the release tarball does.

## Encoding
cyrss uses utf-8 strings internally, but accepts any of the `unicode`/`str`/`bytes` types from Python 2 or Python 3.  `bytes` types can be in any encoding: `cchardet` is used to detect and convert non-utf8 encodings prior to parsing.

## RSS/Atom versions
cyrss detects and handles the various versions of RSS and Atom.  The resulting `CyFeed` and `CyFeedItem` objects have a normalized set of properties from the two standards (see the API section).


## API
cyrss has three main classes: `CyFeed`, `CyFeedItem` and `CyFeedParser`.

### CyFeed
`CyFeed` has the following properties:
* `guid` or `id` [unicode]
* `title` [unicode]
* `description` [unicode]
* `description_type` [unicode]
* `link` [unicode]
* `pub_date` or `pubDate` [unicode]
* `author` [unicode]
* `author_email` [unicode]
* `author_uri` [unicode]
* `items` [Python list of `CyFeedItem` instances]
* `managing_editor` or `managingEditor` [unicode]
* `content_encoded` [unicode]


### CyFeedItem
`CyFeedItem` has the following properties:
* `guid` or `id` [unicode]
* `title` [unicode]
* `description` [unicode]
* `description_type` [unicode]
* `summary` [unicode]
* `summary_type` [unicode]
* `link` [unicode]
* `author` [unicode]
* `author_email` [unicode]
* `author_uri` [unicode]
* `pubDate` or `pub_date` [unicode]
* `enclosure_url` [unicode]
* `enclosure_type` [unicode]
* `content_encoded` [unicode]

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


## Testing

### tl;dr

```bash
make test
```



### General strategy
There are both Python and C++ tests.  Of the two groups, the Python tests are more focused on overall correctness of feed extraction.  The C++ tests are more focused on the lower-level parsing details, especially memory safety and handling of invalid inputs.

As a rough rule, an end-to-end test asserting on the properties of each of a sample feed's 20 entries would belong in the Python test suite.  A test checking that we don't segfault on an invalid RSS version would belong with the C++ tests.

### Python
Python tests are under the root `cyrss_tests` directory.  They live outside of the `cyrss` package due to some oddities in the Cython compilation process.  (It's possible I'm doing something stupid.)
When developing, use the `make test_py` command to run these.  `make test_py` sets an environment variable which enables Cython's compile-on-import feature (pyximport). Without this, you would need to do a full `setup.py install` prior to running the tests.

### C++
The C++ tests use `gtest` and `cmake`, and live under `src/testing`.  To run just the C++ tests, use the make target `test_cpp`.

A version of `gtest` is included in this repo as a submodule, and cmake is configured to look for and use it.  The submodule will be automatically pulled if it's missing.

```bash
git submodule init
git submodule update
```

### Valgrind
There's a separate memory leak check in the C++ testing directory, which parses sample RSS and Atom feeds under Valgrind and reports on any leaks.  This is already run as part of `make test`.  To run it separately, do:

```bash
make memcheck
```

Valgrind often shows some false positives from internal libstdc++ functions.  As long as you see this at the end, we aren't leaking any memory:
```bash
==23459== LEAK SUMMARY:
==23459==    definitely lost: 0 bytes in 0 blocks
==23459==    indirectly lost: 0 bytes in 0 blocks
==23459==      possibly lost: 0 bytes in 0 blocks
```

Valgrind will often show some number of bytes as `still reachable`.  This is fine.


## License

`cyrss` is available under the MIT license.

This library includes a vendored copy of `pugixml`, which is also under the MIT license.

It also contains some code adapted from the open source `newsbeuter` project, which is also licensed under MIT. (Most of that code is no longer recognizable.)


