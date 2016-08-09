import sys

PY3_MODE = True

if sys.version_info[0] == 2:
    PY3_MODE = False

if PY3_MODE:
    str_type = str
    bytes_type = bytes
else:
    str_type = unicode
    bytes_type = str
