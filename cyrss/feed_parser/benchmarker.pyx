import time

__all__ = ('time_calls',)

cpdef time_calls(object py_callable, int n_calls):
    cdef int step_count = 0
    start = time.time()
    while step_count < n_calls:
        py_callable()
        step_count += 1
    end = time.time()
    elapsed = end - start
    return elapsed
