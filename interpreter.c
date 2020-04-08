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
#include "tokenizer.h"

/**
 * A function for opening an input file for reading and an output file for 
 * writing.
 *
 * @param argc Number of elements in the argv array. Should be 3.
 * @param argv An array of pointers to the program name and all the arguments.
 * @return An array containing two
 */
FILE * open_files(int argc, char * argv[]) {
   FILE * files[2];
   if (argc != 3) {
      printf("Usage: interpreter <input_filename> <output_filename>\n");
      exit(1);
   }

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
 * Main function. Most of the program logic is in main. File pointers are 
 * created for input and output. Each line of input is read and parsed into 
 * lexemes. An output file is created detailing the results.
 *
 * @param argc Number of elements in the argv array. Should be 3.
 * @param argv An array of pointers to the program name and all the arguments.
 * @return 0 if the program executed sucessfully.
 */
int main(int argc, char * argv[]) {
   char token[TSIZE];        // storage location for current lexeme
   char input_line[LINE];    // storage location for line of input
   int line_count,           // number of statements read
       start,                // boolean for start of new statement
       count;                // count of tokens on current statement

   line_count = 0;
   start = TRUE;

   // cycles through each line of input
   while (fgets(input_line, LINE, in_file) != NULL) {
      line = input_line;
      if (start) {
         count = 0;
         line_count++;
         fprintf(out_file, "Statement #%d\n", line_count);
         start = FALSE;
      }
      bypass_whitespace();

      // cycles through each character in the current input line
      while (*line != '\0') {
         get_token(token);

         // prints lexeme to out_file if valid
         if (isvalid(token[0], out_file)) {
            start = token[0] == ';' ? TRUE : FALSE;//start=TRUE at new statement
            fprintf(out_file, "Lexeme %d is %s\n", count, token);
            count++;
         }
         bypass_whitespace();
      }

      if (start) {
         fprintf(out_file, DASHES);
      }
   }

   fclose(in_file);
   fclose(out_file);
   return 0;
}

