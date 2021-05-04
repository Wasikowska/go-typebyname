#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <unistd.h>

#include "cachelab.h"

static int verbose, S, E, B;
static char *trace;
static int hit, miss, evict;

static void help() {
  char *helpStr =
      "Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n\
Options:\n\
  -h         Print this help message.\n\
  -v         Optional verbose flag.\n\
  -s <num>   Number of set index bits.\n\
  -E <num>   Number of lines per set.\n\
  -b <num>   Number of block offset bits.\n\
  -t <file>  Trace file.\n\
\n\
Examples:\n\
  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n\
  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n\
";
  printf("%s", helpStr);
}

typedef struct {
  int valid; /* does the line is vacant */
  int dirty; /* does the line contains data that
                have not written to the main memory
                yet */
  int tag;
  int clk; /* the clock num of the last
              reference */
} line;

typedef struct {
  line *pline;
} set;

typedef struct {
  set *pset;
} cache;

static cache c;
static int clk;

static int pow2(int x) {
  int result = 1;
  for (int i = 0; i < x; i++) {
    result *= 2;
  }
  return result;
}

static void initCache() {
  int sn = pow2(S);
  c.pset = (set *)calloc(sn, sizeof(set));
  for (int i = 0; i < sn; i++) {
    c.pset[i].pline = calloc(E, sizeof(line));
  }
}

static void processAddr(long addr) {
  int tag = addr >> (S + B);
  int setIdx = (addr & ((1 << (S + B)) - 1)) >> B;
  set s = c.pset[setIdx];

  // search if the addr is already cached
  for (int i = 0; i < E; i++) {
    if (s.pline[i].valid && s.pline[i].tag == tag) {
      s.pline[i].clk = clk;
      hit++;
      if (verbose) {
        printf(" hit");
      }
      return;
    }
  }

  // addr is not cached, we have a miss
  miss++;
  if (verbose) {
    printf(" miss");
  }

  // find a line for addr
  for (int i = 0; i < E; i++) {
    if (!s.pline[i].valid) {
      s.pline[i].valid = 1;
      s.pline[i].tag = tag;
      s.pline[i].clk = clk;
      return;
    }
  }

  // the set is full, we have to evict a line
  line *candidate = &s.pline[0];
  for (int i = 1; i < E; i++) {
    if (s.pline[i].clk < candidate->clk) {
      candidate = &s.pline[i];
    }
  }

  candidate->clk = clk;
  candidate->tag = tag;

  evict++;
  if (verbose) {
    printf(" eviction");
  }
}

static void processLine(char *line) {
  if (line[0] != ' ') {
    return;
  }

  if (verbose > 0) {
    printf("%s", line);
  }

  // we will ignore the size column, becuase no action cross block
  // boundary
  char action = line[1];
  long addr = 0;
  switch (action) {
  case 'L':
    addr = strtol(line + 3, NULL, 16);
    processAddr(addr);
    break;
  case 'M':
    addr = strtol(line + 3, NULL, 16);
    processAddr(addr);
    processAddr(addr);
    break;
  case 'S':
    addr = strtol(line + 3, NULL, 16);
    processAddr(addr);
    break;
  default:
    fprintf(stderr, "invalid trace file!\n");
    exit(EXIT_FAILURE);
  }

  if (verbose) {
    printf("\n");
  }

  clk++;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    help();
    return 0;
  }

  // the cmdline parsing does not cover all error handlings
  int opt = 0;
  while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
    switch (opt) {
    case 'h':
      help();
      return 0;
    case 'v':
      verbose = 1;
      break;
    case 's':
      S = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      B = atoi(optarg);
      break;
    case 't':
      trace = optarg;
      break;
    default: /* '?' */
      fprintf(stderr, "%s: invalid option -- %s\n", argv[0], optarg);
      help();
      return 0;
    }
  }

  if (S <= 0 || S >= 64 || E <= 0 || E >= 64 || B <= 0 || B >= 64) {
    fprintf(stderr, "Invalid SEB\n");
  }

  FILE *stream;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  stream = fopen(trace, "r");
  if (stream == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  initCache();

  while ((nread = getline(&line, &len, stream)) != -1) {
    if (line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }
    processLine(line);
  }

  free(line);
  fclose(stream);

  printSummary(hit, miss, evict);
  exit(EXIT_SUCCESS);
}
