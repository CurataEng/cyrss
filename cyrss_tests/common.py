import os

def get_data_dir():
    here = os.path.dirname(os.path.realpath(__file__))
    return os.path.realpath(os.path.join(here, '../test_data'))

def in_data_dir(fpath):
    return os.path.join(get_data_dir(), fpath)

def load_test_data(name):
    fpath = in_data_dir(name)
    with open(fpath, 'r') as f:
        rss_data = f.read()
    return rss_data
