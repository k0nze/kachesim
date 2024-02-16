# CMake generated Testfile for Source directory:
# /Users/konze/Programming/mesi_cachesim/tests Build directory:
# /Users/konze/Programming/mesi_cachesim/tests/build
#
# This file includes the relevant testing commands required for testing this
# directory and lists subdirectories to be tested as well.
add_test(
    test_cache_entry_build
    "/opt/homebrew/Cellar/cmake/3.28.2/bin/cmake"
    "--build"
    "/Users/konze/Programming/mesi_cachesim/tests/build"
    "--config"
    "$<CONFIG>"
    "--target"
    "test_cache_entry")
set_tests_properties(
    test_cache_entry_build
    PROPERTIES
        _BACKTRACE_TRIPLES
        "/Users/konze/Programming/mesi_cachesim/tests/CMakeLists.txt;19;add_test;/Users/konze/Programming/mesi_cachesim/tests/CMakeLists.txt;0;"
)
add_test(test_cache_entry "./test_cache_entry")
set_tests_properties(
    test_cache_entry
    PROPERTIES
        FIXTURES_SETUP
        "test_fixture"
        _BACKTRACE_TRIPLES
        "/Users/konze/Programming/mesi_cachesim/tests/CMakeLists.txt;29;add_test;/Users/konze/Programming/mesi_cachesim/tests/CMakeLists.txt;0;"
)
