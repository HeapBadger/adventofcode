#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "day_1.h"

/**
 * @file main.c
 * @brief Entry point for running Advent of Code challenges.
 *
 * Supports running all challenges or a specific day's challenge using
 * command-line arguments.
 */

#define USAGE_GUIDE "Usage: ./adventofcode [<day_1|day_2|...>]"
#define MAX_FILE_PATH_LEN 256
#define ARGS_COUNT 2

/**
 * @brief Structure to map day names to their respective functions.
 */
typedef struct {
  const char *day_name;
  int (*day_function)(const char *file_path);
  int day_number;
} DayChallenge;

/**
 * @brief Runs a specific day's challenge.
 *
 * This function takes a `DayChallenge` structure, which includes the day name
 * and its associated function pointer, and executes the corresponding challenge
 * function. It checks for the existence of `example_<day_number>.txt` and
 * `actual_<day_number>.txt` files in the `data/` directory and runs the
 * function with these files if they exist.
 *
 * @param challenge A `DayChallenge` structure containing the day name,
 * function, and day number.
 */
void run_day(DayChallenge challenge) {
  printf("Running %s challenge...\n", challenge.day_name);

  char example_file[MAX_FILE_PATH_LEN];
  char actual_file[MAX_FILE_PATH_LEN];

  // construct file paths using the day number
  snprintf(example_file, sizeof(example_file), "data/example_%d.txt",
           challenge.day_number);
  snprintf(actual_file, sizeof(actual_file), "data/actual_%d.txt",
           challenge.day_number);

  // check and run the example file if it exists
  if (access(example_file, F_OK) == 0) {
    printf("Found example file: %s. Running...\n", example_file);
    challenge.day_function(example_file);
  } else {
    printf("Example file not found: %s\n", example_file);
  }

  // check and run the actual file if it exists
  if (access(actual_file, F_OK) == 0) {
    printf("Found actual file: %s. Running...\n", actual_file);
    challenge.day_function(actual_file);
  } else {
    printf("Actual file not found: %s\n", actual_file);
  }

  return;
}

/**
 * @brief Entry point of the program.
 *
 * This function handles the logic for running either all challenges or a
 * specific day's challenge based on the command-line argument. It accepts the
 * following arguments:
 * - If no arguments are passed, it runs all the challenges.
 * - If a valid day name (e.g., "day_1") is passed, it runs the corresponding
 * day's challenge.
 *
 * @param argc Argument count, representing the number of command-line
 * arguments.
 * @param argv Argument vector, an array of strings containing the command-line
 * arguments.
 * @return EXIT_SUCCESS if the program runs successfully, EXIT_FAILURE
 * otherwise.
 */
int main(int argc, char *argv[]) {
  int exit_status = EXIT_FAILURE;

  // array of all available challenges
  DayChallenge challenges[] = {
      {"day_1", day_1, 1},
      // Add more days here
  };

  size_t challenge_count = sizeof(challenges) / sizeof(challenges[0]);

  // run all challenges
  if (ARGS_COUNT > argc) {
    printf("Running all Advent of Code challenges...\n");

    for (size_t i = 0; i < challenge_count; i++) {
      run_day(challenges[i]);
    }

    exit_status = EXIT_SUCCESS;
  }

  // print useage guide (too many arguments handed)
  else if (ARGS_COUNT < argc) {
    printf("%s\n", USAGE_GUIDE);
  }

  // run a specific challenge if it exists
  else {
    const char *requested_chal = argv[1];
    int found = false;

    for (size_t i = 0; i < challenge_count; i++) {
      if (0 == strcmp(requested_chal, challenges[i].day_name)) {
        run_day(challenges[i]);
        found = true;
        exit_status = EXIT_SUCCESS;
        break;
      }
    }

    if (!found) {
      printf("Invalid argument: %s\n", requested_chal);
      printf("%s\n", USAGE_GUIDE);
    }
  }

  return exit_status;
}