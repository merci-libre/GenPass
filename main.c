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
  int charset_;
  int uol;
  int mmod; // mod modifier.
} arg;

typedef struct strings {
  int betterstring[256];
} strings;
seeds seed;
strings string;
arg args;

/* Ascii_load: Loads all of the printable characters in ASCII*/
void loadbuffer(int x) {
  int y;
  int c;

  switch (args.uol) { // defined by -x, -u, -m, -n
  case 0:             // do this for spaces and all chars.
    y = 127;
    c = 1;
    break;
  case 1: // caps only. -u
    y = 91;
    c = 1;
    break;
  case 2: // lowercase only. -x
    y = 123;
    c = 1;
    break;
  case 3: // numbers only. -n
    y = 58;
    c = 1;
    break;
  case 4: // upper and lowercase.-m
    y = 91;
    c = 2;
    break;

  default: // print an error message if something goes wrong.
    fprintf(stderr, "Error determining the value of `args.uol`! check "
                    "loadbuffer() function.\n");
    fprintf(stderr, "value of args.uol=%d;\n", args.uol);
    break;
  }

  if (args.verbosity == 1) {
    fprintf(stderr, "starting ascii value loaded: '%c'\n", x);
    fprintf(stderr, "last ascii value loaded: '%c'\n", y - 1);
    fprintf(stderr, "values %d->%d loaded into seed.ascii[]\n ", x, y - 1);
  }
  for (int z = 0; z < c; z++) {
    for (int i = x; i < y; i++) {
      switch (c) {
      case 1:
        seed.ascii[i - x] = i;
        if (args.verbosity == 1) {
          fprintf(stderr, "values %c loaded into seed.ascii[%d]\n ", i, i - x);
        }
        break;
      case 2:
        if (z == 1) {
          seed.ascii[i - x + 26] = i;
          if (args.verbosity == 1) {
            fprintf(stderr, "values %c loaded into seed.ascii[%d]\n ", i,
                    i - x + 26);
          }
        } else {
          seed.ascii[i - x] = i;
          if (args.verbosity == 1) {
            fprintf(stderr, "values %c loaded into seed.ascii[%d]\n ", i,
                    i - x);
          }
        }
        break;
      default:

        fprintf(stderr, "Error loading buffer `seed.ascii[]`! check "
                        "loadbuffer() function.\n");
        break;
      }
    }
    // if doing a combination of numbers/letters. unused otherwise.
    x = x + 32;
    y = y + 32;
  }
}

void ascii_load(int space) {
  int x;
  int finalchar;
  switch (space) {
  case 0: // all characters
    x = 32;
    args.uol = 0;
    loadbuffer(x);
    break;
  case 1: // exclude space -c
    x = 33;
    args.uol = 0;
    loadbuffer(x);
    break;
  case 2: // caps only -u
    x = 65;
    args.uol = 1;
    loadbuffer(x);
    break;
  case 3: // lowercase only -x
    x = 97;
    args.uol = 2;
    loadbuffer(x);
    break;
  case 4: // numbers only -n
    x = 48;
    args.uol = 3;
    loadbuffer(x);
    break;
  case 5: // mix upper and lower -m
    x = 65;
    args.uol = 4;
    loadbuffer(x);
    break;
  default:
    fprintf(stderr, "Error! check ascii_load() function.\n");
    break;
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
        int z = rand() % args.mmod;
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
    fprintf(stderr, "Random number modifier=%d\n", args.mmod);
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
  ascii_load(args.charset_);

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
  fprintf(stderr, "CHARACTER GENERATION OPTIONS:\n-c : removes spaces as a "
                  "character (character "
                  "32).\n-u : uppercase letters only\n-x : lowercase letters "
                  "only \n-m : does a mix of upper and lowercase letters only "
                  "\n-n : numbers only\n\nMAIN OPTIONS:\n-l <integer> : "
                  "length of password generated.\n"
                  "-v : be verbose.\n-h : print help menu.\n");
}

/* Enumarate arguments, and parse user input for invalid input.*/
int main(int argc, char *argv[]) {
  int option;
  args.mmod = 94;
  // defined by -x, -u, -m, -n
  while ((option = getopt(argc, argv, "cuxnmvl:h")) != -1) {
    switch (option) {
    /* CHARACTER OPTIONS */

    // characters only (no spaces)
    case 'c':
      args.charset_ = 1;
      args.mmod = 93;
      break;
    case 'u': // uppercase
      args.charset_ = 2;
      args.mmod = 26;
      break;
    case 'x': // lowercase
      args.charset_ = 3;
      args.mmod = 26;
      break;
    case 'n': // numbers only
      args.charset_ = 4;
      args.mmod = 10;
      break;
    case 'm': // mixed chars.
      args.charset_ = 5;
      args.mmod = 52;
      break;

    /* MAIN OPTIONS */
    // length
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
    // help menu
    case 'h':
      printhelp();
      exit(2);
      break;
    // verbosity
    case 'v':
      args.verbosity = 1;
      break;
    // invalid argument.
    default:
      printhelp();
      exit(2);
      break;
    }
  }
  // no argument given.
  if (argc <= 1) {
    printhelp();
  }
  // generate the string.
  return 0;
}
