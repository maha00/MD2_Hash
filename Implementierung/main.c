// Necessary Imports
#include <string.h>
// For the Monotonic Clock
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#define _GNU_SOURCE
// Including the Header Files.
#include "blocklauf-padding.h"
#include "io.h"
#include "md2_hash.h"
#include "md2_alternative.h"
// Librarys being used.
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*EXAMPLE USAGE:

./test (-(V (0|1) | B (1|2|3|4|5))) FILENAME

*/

// macro copied from: CFEngine blog
#define OPTIONAL_ARGUMENT_IS_PRESENT                           \
  ((optarg == NULL && optind < argc && argv[optind][0] != '-') \
       ? (bool)(optarg = argv[optind++])                       \
       : (optarg != NULL))

// Default options.
int option_v = 0;
int option_b = 0;
char *filename;

// Help message for the Options.
// help message: TODO: Programmbeschreibung
const char *help_msg =
    "NAME\n"
    " \tThis program computes the hashfunction of a file according to the MD2-Algorithm. \n"
    "SYNOPSIS\n"
    "\t./test [option] <filename>\n"
    "DESCRIPTION\n"
    "\n"
    "    Fileinput as the last argument is mandatory\n"
    "\n"
    " \t -V  X       choose which implementation to use (default: X = 0, alternate: X = 1)\n"
    "\t\tif argument is not set, default will be used\n"
    " \t -B [X]     measure elapsed time for execution. Optional argument: execution runs\n"
    " \t -h, --help    \n"
    "\t\tdisplay this help and exit\n"
    "EXAMPLES\n"
    "\t./test -B test_empty_file.txt\n"
    "\t\trun programm on test.txt and display time elapsed\n"
    "\t./test -V 1 -B 3 test_long_input.txt\n"
    "\t\trun alternate programm on test.txt 3 times and display time elapsed \n";

// Function to print help message
void print_help()
{
  fprintf(stderr, "\n%s", help_msg);
}
// Running the Hash Algorithm.
void md2(const uint8_t *buff, size_t size, uint8_t result[16])
{
  printf("Testing with Main Implementation\n");
  init();
  md2_hash(size, buff, result);
}
// Running the Alternative Hash Algorithm.
void md2_alternative(const uint8_t *buff, size_t size, uint8_t result[16])
{
  MD2_HASH hash;
  printf("Testing with Alternative Implementation\n");
  initialize_alt(&hash);
  md2_update_alt(&hash, buff, size);
  size = (&hash)->length;
  md2_hash_alt(result, &hash);
}

// Controlling if the results are correct.
int md2_test(int implementation)
{
  // Result allocated in this array
  uint8_t result[16];
  // Initiantiate test buffer and its size
  const uint8_t *buff;
  size_t size = 0;
  // Expected Results in Hexadecimal Form.
  //  expected result of the MD2_Hash Algorithm as indicated in the documentation:"https://datatracker.ietf.org/doc/html/rfc1319"
  uint8_t expected_short_input[16] = {0xda, 0x85, 0x3b, 0x0d, 0x3f, 0x88, 0xd9, 0x9b, 0x30, 0x28, 0x3a, 0x69, 0xe6, 0xde, 0xd6, 0xbb}; // "abc"
  uint8_t expected_long_input[16]= {0xda,0x33,0xde,0xf2,0xa4,0x2d,0xf1,0x39,0x75,0x35,0x28,0x46,0xc3,0x03,0x38,0xcd};//"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
  uint8_t expected_empty_input[16]={0x83,0x50,0xe5,0xa3,0xe2,0x4c,0x15,0x3d,0xf2,0x27,0x5c,0x9f,0x80,0x69,0x27,0x73}; // "" -> empty string 
  uint8_t expected_input_500[16] = {0x3d,0x0d,0xe8,0x00,0xd4,0xb3,0x3e,0x74,0xfd,0x2a,0x26,0x00,0xaa,0xa3,0x91,0xaf};
  uint8_t expected_input_1000[16] = {0xcc,0xd6,0xe8,0xc4,0x6c,0x23,0x88,0xba,0x50,0xd1,0xf8,0x41,0x8a,0x08,0x4c,0x19};
  uint8_t expected_input_1250[16] = {0x4b,0x31,0x5e,0x07,0xf3,0xa8,0x1b,0xff,0x99,0x4d,0xe1,0x52,0x0c,0xeb,0x89,0x2a};
  uint8_t expected_input_1500[16] = {0x17,0x99,0x43,0xbd,0x31,0x82,0xce,0x6c,0xa4,0x8e,0x7e,0xc1,0x6d,0x03,0xea,0x7e};
  uint8_t expected_input_2500[16] = {0xa7,0x23,0xd7,0x67,0x84,0x2b,0xfe,0xa0,0xc9,0x80,0x72,0x3d,0x72,0x5c,0x01,0x3f};
  uint8_t expected_input_5000[16] = {0xa0,0xf2,0x7c,0x43,0x6a,0x00,0xc1,0x9a,0x6a,0x26,0x9c,0xd6,0x93,0x0b,0x2d,0x1d};
  uint8_t expected_input_10000[16] = {0x6d,0x47,0x9b,0x03,0x5c,0x5d,0xf9,0xc9,0xd6,0x66,0x08,0xe4,0x4b,0x3f,0xda,0x9b};
  uint8_t expected_input_25000[16] = {0x69,0x8c,0xb6,0xa0,0xba,0xd2,0xea,0x99,0x5f,0x6b,0x0f,0x52,0x4d,0x69,0xad,0xfc};
  uint8_t expected_input_100000[16] = {0xfd,0x24,0xfb,0x9f,0xd3,0x05,0x62,0xe4,0x50,0x66,0x62,0xca,0x12,0x12,0x83,0xb4};
  uint8_t expected_morse_code[16] = {0xc6,0xdc,0x54,0xd6,0x33,0x08,0x7c,0xbe,0x50,0x46,0xd2,0x51,0x29,0xe1,0x46,0x7c};
  uint8_t expected_input_100[16]={0x01,0xb1,0xe5,0x2f,0x9b,0xba,0xc5,0x33,0x14,0xf9,0x45,0x74,0x04,0xe3,0x40,0xcd}; 

  // initialize the test boolean
  int verif = 1;

  // Running the program :initializing the buffer containing the text read from the file and sequentially calling the methods from md2_hash.c
  buff = read_file(filename, &size);
  if (implementation == 0)
  {
    md2(buff, size, result);
    //Freeing the Memory.
    free((char*)buff);
  }
  else if (implementation == 1)
  {
    md2_alternative(buff, size, result);
  }
  else
  {
    printf("no implementation chosen, please provide -V 0 or -V 1");
    goto end;
  }
  /*init ();
  md2_update(buff,size);
  md2_hash(size, buff, result);*/

  // test that passes if the expected encrypted md2 digests is equal to the result in all test cases
 verif &= (!memcmp(expected_empty_input, result, 16)||!memcmp(expected_short_input, result, 16)
 ||!memcmp(expected_long_input, result, 16)||!memcmp(expected_input_500, result, 16)||
 !memcmp(expected_input_1000, result, 16)||!memcmp(expected_input_10000, result, 16)||
 !memcmp(expected_input_25000, result, 16)||!memcmp(expected_input_100000, result, 16)||
 !memcmp(expected_morse_code, result, 16)||!memcmp(expected_input_1250, result, 16)||
 !memcmp(expected_input_1500, result, 16)||!memcmp(expected_input_5000, result, 16)||
 !memcmp(expected_input_2500, result, 16)||!memcmp(expected_input_100, result, 16)); 

  // Method that prints out the input and the result in hexadecimal format
  print(result);
end:
  return verif;
}

int main(int argc, char **argv)
{

  

  // Struct for long arguments
  static const struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                               {0}};
  int index = -1;
  int result = getopt_long(argc, argv, "V:B::h", long_options, &index);
  // Given Option 'h' brings us to help.
  if (result == 'h')
  {
    print_help();
    return 0;
  }
  filename = argv[argc - 1];
  // prove if filename is valid. If not, exit with error
  if (argv[1] == NULL || access(filename, F_OK) != 0)
  {
    printf("Please provide a valid filename as the last argument.\n");
    return 1;
  }
  // go through the options
  while (result != -1)
  {

    switch (result)
    {

    // Version
    case 'V':
      if (*optarg == '-' || !isdigit(*optarg))
      { // check if there is a valid option after -V, exit with error if not
        printf("Missing argument for -V. Try --help for possible options\n");
        return 0;
      }
      else
      {
        option_v = atoi(optarg); // V has a valid option, overwrite default value
        break;
      } // TODO
    // Measuring the time.
    case 'B':
      if (OPTIONAL_ARGUMENT_IS_PRESENT && isdigit(*optarg))
      {                          // check if -B has an argument
        option_b = atoi(optarg); // if yes, then overwrite default value
        if (option_b < 1)
        {
          printf("Running -B with < 1 is not possible.\n");
          return -1;
        } //
      }
      else
      { // default in case -B exists but without argument
        option_b = 1;
      }
      break;
    // Unknown option
    case '?': // error if unknown option is typed
      printf("You entered an unknown option. Try --help for possible "
             "options\n");
      return 0;
      break;
    }
    result = getopt_long(argc, argv, "V:B::h", long_options, &index);
  }
  // Start clock to measure time to run function with Monotonic Clock.
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);
  // default: run programm one time
  int program_runs = 1;
  if (option_b > 1)
  { // if option -B is set, take argument for counter
    program_runs = option_b;
  }
  // Iterating until the number of given runs.
  for (int i = 0; i < program_runs; i++)
  {
    // checks option after -V
    switch (option_v)
    {
    case 0: // default programm
            //  md2_test();
      printf("MD2_test: %s\n", md2_test(0) ? "SUCCESS" : "FAIL");
      break;

    case 1: // alternativ programm with -V 1
      printf("MD2_test_alternative: %s\n", md2_test(1) ? "SUCCESS" : "FAIL");
      break;

    default: // If argument after -V is not 0 or 1 -> exit with error
      printf("Please provide a valid version for -V.\n");
      return -1;
    }
  }
  // Measuring the time.
  if (option_b > 0)
  { // if option -B is set, stop the clock and print elapsed time

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
    double avg_time = time / program_runs;

    printf("Elapsed time: %f\n", time);
    if (program_runs > 1)
    {

      printf("Elapsed average time: %f\n", avg_time);
    }
  }
}