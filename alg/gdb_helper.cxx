#include "gdb_helper.h"
#include <stdarg.h>
#include <stdio.h>

extern "C" {
int gh_fprintf(const char *path, const char *format, ...) {
  FILE *file = fopen("abc", "w");
  if (file == 0) {
    perror("fopen in gdb_fprintf");
    return -1;
  }

  va_list ap;
  va_start(ap, format);
  int r = vfprintf(file, format, ap);
  if (r < 0) {
    perror("vfprintf in gdb_fprintf");
  }
  va_end(ap);
  return r;
}

void gh_cf(int i) { printf("gh_cf: i = %d\n", i); }
}

void gh_cppf(int i) { printf("gh_cppf: i = %d\n", i); }

namespace Namespace {

void gh_nscppf(int i) { printf("gh_nscppf: i = %d\n", i); }

} // namespace Namespace
