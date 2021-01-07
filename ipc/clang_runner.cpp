#include <sched.h>
#include <string>
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

std::string source = R"(
#include <stdlib.h>

int main() {
  printf("hello, clang runner");
  return 0;
}
)";

void trace(const char* str) {
  printf("RUNNER: %s\n", str);
}

// IT IS VERY IMPORTANT THAT WE CLOSE EVERY FD IN ALL PROCESSES!!!
// https://stackoverflow.com/questions/24766013/is-it-really-necessary-to-close-the-unused-end-of-the-pipe-in-a-process
void clang() {
  int infd[2] = {-1, -1};
  int outfd[2] = {-1, -1};

  pipe(infd);
  pipe(outfd);

  if (pid_t child = fork() != 0) {
    // feed input
    trace("feed input to infd[1]");
    if (write(infd[1], source.c_str(), source.size()) == -1) {
      perror("write(infd[1], source.c_str(), source.size()");
      return;
    }

    close(infd[1]);
    close(infd[0]);
    close(outfd[1]);

    // get output
    trace("read output from outfd[0]");
    char buf[1025];
    ssize_t size = read(outfd[0], buf, 1024);
    while (size != 0) {
      if (size == -1) {
	perror("read(outfd[0], buf, 1024)");
	break;
      }

      buf[size] = 0;
      printf("->get output from clang : size = %zd\n", size);
      size = read(outfd[0], buf, 1024);
    }
    close(outfd[0]);
  } else {
    // redirect io
    if (dup2(infd[0], STDIN_FILENO) == -1) {
      perror("dup2(infd[0], 0)");
      return;
    }
    if (dup2(outfd[1], STDOUT_FILENO) == -1) {
      perror("dup2(outfd[1], 1)");
      return;
    }

    close(infd[0]);
    close(infd[1]);
    close(outfd[0]);
    close(outfd[1]);

    //run clang
    printf("WRAPPER: run clang\n");
    if (execlp("clang-10", "clang-10", "-x", "c", "-c", "-o", "-", "-", NULL) == -1) {
      perror("execlp");
    }
  }
}
