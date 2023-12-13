/*
 * The password generator works like so
 *
 * 1. An array called 'characters' is created, there can be custom set
 * characters defined in text files, OR use a standard ASCII/UTF-8 character
 * set.
 * 2. each character is generated as a random number is seeded and multiplied by
 * the one's digit of seconds of the current time:
 *
 * i.e take the string:
 *
 * 3f-30c94m.Sfme
 *      ^------------ the generation for this letter
 *                    would be as follows:
 *
 *                    int seed
 *                    int p = time (second)
 *                    char character[x]
 *
 *
 *                    character_select = (p*random)%16.
 *                     |
 *                     |
 *                     |
 *                     |--------> seeds to a 4*4 matrix, filled from random
 * numbers based on the amount of characters.
 *                                    ||
 *                                    ||            x=random%128, where 32<x<127
 *                                    ||
 *                                    ||==========> |34|43|54|95|
 *                                                  |72|23|44|55|
 *                  the number generated <========= |52|41|33|24|
 *                  is used as a reference          |43|36|15|_x|
 *                  point to select a
 *                  corresponding integer.
 *                  refered to as character[x]
 *
 *
 *
 * EVALUATE:
 * - possible vulnerability with using seconds, needs to be more complex.
 *   (p*random*)
 *
 *
 *
 *
 */
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
