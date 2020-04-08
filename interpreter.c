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
#include "parser.h"


/* Global variables */
char * line;                 // pointer to the next character to process


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
   FILE * in_file = NULL;    // pointer to input file
   FILE * out_file = NULL;   // pointer to output file

   if (argc != 3) {
      printf("Usage: interpreter <input_filename> <output_filename>\n");
      exit(1);
   }

   in_file = fopen(argv[1], "r");
   if (in_file == NULL) {
      fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
      exit(1);
   }

   out_file = fopen(argv[2], "w");
   if (out_file == NULL) {
      fprintf(stderr, "ERROR: could not open %s for writing\n", argv[2]);
      exit(1);
   }

   // cycles through each line of input
   while (fgets(input_line, LINE, in_file) != NULL) {
      line = input_line;
      fprintf(out_file,"%s\n",input_line);
      int result = bexpr(token); 
      if(result == SYN_ERR){
         fprintf(out_file,"===> '%s' expected\nSyntax Error\n\n", expected_var);
      }
      // bypass_whitespace();

      // // cycles through each character in the current input line
      // while (*line != '\0') {
      //    get_token(token);
      //    bypass_whitespace();
      // }
   }

   fclose(in_file);
   fclose(out_file);
   return 0;
}

