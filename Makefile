test_deps:
	bash scripts/build_test_deps.sh

build_cpp_tests: test_deps
	mkdir -p build/cpp && cd build/cpp && cmake ../../ && make -j4

clean:
	rm -rf build dist
	rm -f cyrss/feed_parser/*.cpp

test_cpp: build_cpp_tests
	./build/cpp/test_runner

test_py:
	CYRSS_TESTING_USE_PYXIMPORT=1 py.test cyrss_tests/*.py

test: test_cpp test_py

release: clean
	cython --cplus cyrss/feed_parser/*.pyx
	python setup.py sdist

.PHONY: test clean test_py test_cpp build_cpp_tests

