#pragma once

// these functions are gdb helpers, they should be packed into a
// shared library and loaded manually when we need them. in some gdb
// debugging session, we can call these functions to get a better
// debugging experience.
extern "C" {

// gc_cf, gh_cppf, gh_nscppf all works, but gh_fprintf fails to
// work.
//
// show language returns c++, so gdb cares name mangling. var-sized
// arguments functions may have some odds that when gdb calls them,
// a segmentation fault occurs
int gh_fprintf(const char *path, const char *format, ...);

void gh_cf(int i);
}

void gh_cppf(int i);

namespace Namespace {
void gh_nscppf(int i);
}
