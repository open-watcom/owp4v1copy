
This directory and its subdirectories contain the regression tests for
the C++ compiler and its corresponding runtime library. The tests can be
run in a largely automated manner. In the regress directory the command
script 'onetest.cmd' runs all tests once with "typical" compiler options
selected. The command script 'testrun.cmd' runs the tests multiple times
using, for example, different compiler options in each run. Note that
fully processing testrun.cmd takes a considerable amount of time---even
on a fast machine.

The subdirectories of regress contain the individual test suites. Each
test suite contains a 'onetest.cmd' and a 'testrun.cmd' command script
that exercises that test suite only. Running one test suite is naturally
much quicker than running the entire collection of regression tests in
one session.

The output of the command scripts are log files in the bld\plustest
directory. Each time a test is run a line is entered in the log file for
that test containing a time stamp. If the test is successful the word
"PASS" is entered on the next line. If the test is unsuccessful nothing
is written to the log besides the time stamp.

Of particular interest are the suites regress\positive and
regress\diagnose. The positive tests exercise the compiler's handling of
correct C++ language constructs. The diagnose tests, on the other hand,
explore the compiler's ability to detect incorrect programs. The other
test suites under regress exercise the compiler and its libraries in
other ways.

Note that these are *regression* tests and not, for example, standards
conformance tests. At any given time they are all supposed to work. If
any of the tests fail the matter should be investigated and corrected;
it means that something that used to work no longer works. Some tests
have material commented out or extra "bug" files that are not currently
part of the test. Such code doesn't work but should. Once whatever bug
it demonstrates is fixed that code should become an active part of its
test suite. In general if a new feature is added to the compiler or if a
bug is fixed, an appropriate test should be added here so exercise that
feature or fix. In this way if the new feature or fix later breaks the
error will be detected.

{ To do: Describe how to add tests }
