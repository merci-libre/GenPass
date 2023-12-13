#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define ROWS 5
#define COLS 5

typedef struct seeds {
  int ascii[94];
  int newseed;
} seeds;

typedef struct strings {
  int betterstring[25];
} strings;
seeds seed;
strings string;

void ascii_load() {
  int x = 0;
  for (int i = 32; i < 127; i++) {
    seed.ascii[i - 32] = i;
  }
}
void string_generator() {
  int charvalue;
  int z;

  // init matrix
  int matrix[ROWS][COLS];
  srand(time(NULL) * getpid() + seed.newseed);

  /*fill the table up:*/
  for (int x = 0; x < ROWS; x++) {
    for (int y = 0; y < COLS; y++) {
      int z = rand() % 94;
      matrix[x][y] = seed.ascii[z];
    }
  }
  // add all numbers in matrix together for next seed.

  for (int x = 0; x < ROWS; x++) {
    fprintf(stderr, "\n");
    for (int y = 0; y < COLS; y++) {
      // printing the table
      fprintf(stderr, "[%d]", matrix[x][y]);
      string.betterstring[x * COLS + y] = matrix[x][y];
      seed.newseed = matrix[x][y] * matrix[x][y] % getpid() + time(NULL) +
                     (seed.newseed >> 1);
    }
  }

  fprintf(stderr, "\n");
  fprintf(stderr, "Generated String: ");
  for (int i = 0; i < ROWS * COLS; i++) {
    fprintf(stderr, "%c", string.betterstring[i]);
    fprintf(stdout, "%c", string.betterstring[i]);
  }
  fprintf(stderr, "\n\n");
  fprintf(stdout, "\n");
}

int main() {
  // load the ascii table.
  ascii_load();
  // generate the string.
  string_generator();
  return 0;
}
