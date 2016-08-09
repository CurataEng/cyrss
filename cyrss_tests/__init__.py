import os

def should_use_pxyimport():
    return bool(os.getenv('CYRSS_TESTING_USE_PYXIMPORT'))


if should_use_pxyimport():
    import pyximport
    pyximport.install()
