#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// clang usage:
// - read from llvm ir :: clang -x ir test.ll
// - compile to object file :: clang test.c -c
// - compile to assembly :: clang test.c -S
// - compile to bytecode :: clang test.c -emit-llvm
// - compile to llvm ir text :: clang test.c -emit-llvm -S
// - read from stdin :: clang -x ir -
// - write to stdout :: clang test.c -o -

void clang() {

  // load test.ll
  int fd = open("./test.ll", O_RDONLY);

  char buf[1024];
  ssize_t size = read(fd, buf, 1024);

  int input[2];
  pipe(input);

  close(fd);


  
  
}
