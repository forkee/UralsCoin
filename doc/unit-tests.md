Compiling/running unit tests
------------------------------------

Unit tests will be automatically compiled if dependencies were met in configure
and tests weren't explicitly disabled.

After configuring, they can be run with 'make check'.

To run the uralsd tests manually, launch src/test/test_urals .

To add more uralsd tests, add `BOOST_AUTO_TEST_CASE` functions to the existing
.cpp files in the test/ directory or add new .cpp files that
implement new BOOST_AUTO_TEST_SUITE sections.

To run the urals-qt tests manually, launch src/qt/test/urals-qt_test

To add more urals-qt tests, add them to the `src/qt/test/` directory and
the `src/qt/test/test_main.cpp` file.
