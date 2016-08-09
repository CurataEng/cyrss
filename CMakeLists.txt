cmake_minimum_required(VERSION 2.8)
project(cyrss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/googlemock/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/build/googlemock
)

set(LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/xml/pugixml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/pugi_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/FeedParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/atom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/rss.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/time_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/url_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Feed.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util.cpp

)

add_executable(test_runner
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/test/test_pugi_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/test/test_FeedParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/test/test_rss_parsing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/test/test_atom_parsing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/test/test_time_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/testing/run_tests.cpp
    ${LIB_SOURCES}
)

target_link_libraries(test_runner
    gmock
    pthread
    atomic
)

