
OWSTL Benchmark README
======================

This directory contains benchmark programs that exercise various aspects
of Open Watcom STL. The intent of these programs is to provide data for
the profiler so that "performance bugs" can be noticed and addressed.
These programs are not intended to exercise the facilities of OWSTL in a
realistic way. Instead each program is focused on one individual
facility (or on a small set of closely related facilities) with the
intent of helping that one facility become optimized.

Unlike the other benchmarks, the makefile here only compiles the
programs. It does not run them. The idea is that each program would be
run individually as needed. It might make sense to have a make option
that would run all the programs under wsample, but probably that option
shouldn't be the default.

Note that the programs here are compiled with the -d2 option. This
inhibits optimization somewhat, but it is necessary to get good results
in the profiler (an unfortunate, but probably inescapable, circum-
stance).
