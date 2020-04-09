/**
 * Driver for the Interpreter project. Calls frunctions from parser.c and 
 * tokenizer.c to parse and interpret a text file.
 *
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2020-04-13
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "interpreter.h"


/* Global variables */
char * line;                 // pointer to the next character to process

/**
 * Main function. Runs the interpreter.
 *
 * @param argc Number of elements in the argv array. Should be 3.
 * @param argv An array of pointers to the program name and all the arguments.
 * @return 0 if the program executed sucessfully.
 */
int main(int argc, char * argv[]) {
   FILE ** files;
   usage(argc);
   files = open_files(argv);
   tokenize(files[0], files[1]);
//   parse(files[0], files[1]);
   close_files(files);
   return 0;
}

/**
 * This function acts as a syntax recognizer and symantic recognizer.
 *
 * @param in_file A pointer to the input file.
 * @param out_file A pointer to the output file.
 */
//int parse(FILE * in_file, FILE * out_file) {
//   char token[TSIZE];        // storage location for current lexeme
//   char input_line[LSIZE];   // storage location for line of input
//
//   // cycles through each line of input
//   while (fgets(input_line, LSIZE, in_file) != NULL) {
//      line = input_line;
//      fprintf(out_file, "%s\n", input_line);
//      int result = bexpr(token); 
//      if(result == SYN_ERR){
//         fprintf(out_file,"===> '%s' expected\nSyntax Error\n\n", expected_var);
//      }
//      bypass_whitespace();
//
//      // cycles through each character in the current input line
//      while (*line != '\0') {
//         get_token(token);
//         bypass_whitespace();
//      }
//   }
//   return 0;
//}

/**
 * This function acts as a lexical recognizer. It parses the input file into 
 * lexemes. Output is presented statement by statement.
 *
 * @param in_file A pointer to the input file.
 * @param out_file A pointer to the output file.
 */
void tokenize(FILE * in_file, FILE * out_file) {
   char token[TSIZE];        // storage location for current lexeme
   char input_line[LSIZE];   // storage location for line of input
   int line_count,           // number of statements read
       start,                // boolean for start of new statement
       count;                // count of tokens on current statement

   line_count = 0;
   start = TRUE;

   // cycles through each line of input
   while (fgets(input_line, LSIZE, in_file) != NULL) {
      line = input_line;
      if (start) {
         count = 0;
         line_count++;
         fprintf(out_file, "Statement #%d\n", line_count);
         start = FALSE;
      }
//      bypass_whitespace();

      // cycles through each character in the current input line
      while (*line != '\0') {
         get_token(token);

         // prints lexeme to out_file if valid
         if (isvalid(token[0], out_file)) {
            start = token[0] == ';' ? TRUE : FALSE;//start=TRUE at new statement
            fprintf(out_file, "Lexeme %d is %s\n", count, token);
            count++;
         }
//         bypass_whitespace();
      }

      if (start) {
         fprintf(out_file, DASHES);
      }
   }
}

/**
 * Checks the amount of command line arguments.
 *
 * @param argc Number of elements in the argv array. Should be 3.
 */
void usage(int argc) {
   if (argc != 3) {
      printf("Usage: interpreter <input_filename> <output_filename>\n");
      exit(1);
   }
}

/**
 * Opens an input file for reading and an output file for writing.
 *
 * @param argv An array of pointers to the program name and all the arguments.
 * @return An array of file pointers.
 */
FILE ** open_files(char ** argv) {
   FILE ** files;
   files = (FILE **)calloc(2, sizeof(FILE *));

   files[0] = fopen(argv[1], "r");
   if (files[0] == NULL) {
      fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
      exit(1);
   }

   files[1] = fopen(argv[2], "w");
   if (files[1] == NULL) {
      fprintf(stderr, "ERROR: could not open %s for writing\n", argv[2]);
      exit(1);
   }
   return files;
}

/**
 * Closes the input file and output file.
 *
 * @param files An array of file pointers.
 */
void close_files(FILE ** files) {
   fclose(files[0]);
   fclose(files[1]);
}
