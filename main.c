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
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// add user defined rows and columns?
#define ROWS 8
#define COLS 8

typedef struct seeds {
  int ascii[94];
  int newseed;
  int generated;
} seeds;

typedef struct arg {
  int verbosity;
} arg;

typedef struct strings {
  int betterstring[256];
} strings;
seeds seed;
strings string;
arg args;

/* Ascii_load: Loads all of the printable characters in ASCII*/
void ascii_load() {
  int x = 0;
  for (int i = 32; i < 127; i++) {
    seed.ascii[i - 32] = i;
  }
}
/* Gordian Knot algorithm */
void string_generator(int matrix[ROWS][COLS],
                      int length) { // Where the magic happens.
  int iterations;

  // check the value of length to properly iterate the length of
  // repeated matrices
  if (length <= ROWS * COLS) {
    iterations = 1;
  } else if (length % (ROWS * COLS) != 0) {
    iterations = (length / (ROWS * COLS) + 1);
  } else {
    iterations = (length / (ROWS * COLS));
  }

  /* HOW IT WORKS:
   * For each iteration, a new seed is generated based on 3 values,
   *
   * F(s)=current time * Process ID + s * iterations
   * Where the previous seed is reseeded back into the random number generator.
   *
   * The value is loaded into srand, and the random number generator
   * loads the ascii value from the 94 printable characters and then seeds it
   * back into s.
   *
   * string_generator(): // the 'gordian knot'.
   *                          [do 16 times]
   *       |->F(s)->srand(s)--|-|-|-|-|-|-|->rand()%94
   *       |        |->new 's'               |->ascii char---->matrix(x,y)
   *       |----------------|
   *
   * This way, through each iteration, the matrix is reloaded with new
   * random characters through each iteration.
   *
   * Once the matrix is loaded, it fills the value of string.betterstring
   * with characters based on the length defined by the user.
   *
   * genpass() length->sandwich()------------>betterstring[length]
   *               |                          ^
   *               |                          |
   *               |->matrix[x][y]->string_generator()
   *
   * */

  for (int z = 0; z < iterations; z++) {
    // generate the seeds
    seed.generated = time(NULL) * getpid() + seed.newseed * z;
    srand(seed.generated);
    // debugging
    if (args.verbosity == 1) {
      fprintf(stderr, "\ngenerated seed=%d\n", seed.generated);
      fprintf(stderr, "\niteration=%d\n", z + 1);
    }
    // Load the matrix with characters.
    for (int x = 0; x < ROWS; x++) {
      for (int y = 0; y < COLS; y++) {
        int z = rand() % 94;
        matrix[x][y] = seed.ascii[z];
      }
    }

    for (int x = 0; x < ROWS; x++) {
      if (args.verbosity == 1) {
        fprintf(stderr, "\n");
      }
      for (int y = 0; y < COLS; y++) {
        // printing the table
        if (args.verbosity == 1) {
          fprintf(stderr, "[%c]", matrix[x][y]);
        }
        // truncate the string
        string.betterstring[(z * ROWS * COLS) + (x * COLS + y)] = matrix[x][y];
        seed.newseed = getpid() * 4096;
      }
    }
  }
  if (args.verbosity == 1) {
    fprintf(stderr, "\n");
    fprintf(stderr, "Generated String: ");
  }
}
/* sandwich()
 *
 * The purpose of sandwich() is to print the string out and create a matrix.
 *
 * */
void sandwich(int length) {
  int charvalue;
  int z;

  // init matrix
  int matrix[ROWS][COLS];
  // Load the ascii table's printable characters.
  ascii_load();
  string_generator(matrix, length);
  for (int i = 0; i < length; i++) {
    if (args.verbosity == 1) {
      fprintf(stderr, "%c", string.betterstring[i]);
    }
    fprintf(stdout, "%c", string.betterstring[i]);
  }

  // Remove trailing NULLs
  if (args.verbosity == 1) {
    fprintf(stderr, "\n\n");
  }

  fprintf(stdout, "\n");
}

/*
 * Help command
 */

void printhelp() {
  fprintf(stderr, "usage: genpass -l <LENGTH> | genpass [OPTION] <argument>\n");
  fprintf(stderr, "OPTIONS:\n-l <integer> : length of password generated.\n"
                  "-v : be verbose.\n-h : print help menu.\n");
}

/* Enumarate arguments, and parse user input for invalid input.*/
int main(int argc, char *argv[]) {
  int option;
  while ((option = getopt(argc, argv, "vl:h")) != -1) {
    switch (option) {
    case 'l':
      // if the argument is less than 0 or is 0,
      if ((strcmp(optarg, "0") == 0) || atoi(optarg) <= 0) {
        fprintf(stderr, "Cannot create a string length of zero\n");
        exit(2);
      }
      if (!isdigit(*optarg)) {
        fprintf(stdout,
                "ERROR: %s after -l is not an integer... exiting gracefully.\n",
                optarg);
        exit(2);
      }
      if (atoi(optarg) <= 256) {
        sandwich(atoi(optarg));
      } else {
        fprintf(stdout, "%s is greater than 256, exiting gracefully.\n",
                optarg);
        exit(2);
      }
      break;
    case 'h':
      printhelp();
      exit(2);
      break;
    case 'v':
      args.verbosity = 1;
      break;
    default:
      printhelp();
      exit(2);
      break;
    }
  }
  if (argc <= 1) {
    printhelp();
  }
  // generate the string.
  return 0;
}
