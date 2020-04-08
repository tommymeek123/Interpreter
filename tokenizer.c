/**
 * Tokenizer - A simple token recognizer. This program acts as a lexical 
 * recognizer. It analyzes a text file and parses its contents into lexemes. 
 * It then catagorizes each lexeme into a type of token. Output is presented 
 * statement by statement.
 *
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2020-03-20
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenizer.h"

/* Global variables */
extern char * line;

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
   int line_count,           // number of statements read
       start,                // boolean for start of new statement
       count;                // count of tokens on current statement

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

/**
 * This function determines the next complete lexeme in the input stream. It 
 * then puts that token in the location pointed to by token_ptr.
 *
 * @param token_ptr A pointer to a memory location used to store a lexeme.
 */
void get_token(char * token_ptr) {
   *token_ptr = *line;
   line++;
   switch (*token_ptr) {
      case '+':              // For these cases, the complete
      case '-':              // lexeme is a single character
      case '*':              // and no further work is 
      case '/':              // required apart from ending 
      case '(':              // the string in token_ptr 
      case ')':              // with a null terminating character.
      case '^':
      case ';':
         break;
      case '=':              // For these cases, the complete
      case '<':              // lexeme is a single character
      case '>':              // unless followed by an '='.
      case '!':              // in this case, it is two characters.
         if (*line == '=') {
            token_ptr++;
            line++;
            *token_ptr = '=';
         }
         break;
      case '0'...'9':
         // appends digits to the lexeme until a non-numeric character is found
         while (isdigit(*line)) {
            token_ptr++;
            *token_ptr = *line;
            line++;
         }
         break;
      default:
         *token_ptr = '@';   // '@' is used to denote an invalid lexeme.
         break;
   }
   token_ptr++;
   *token_ptr = '\0';
}

/**
 * Moves the line pointer to ignore whitespace.
 */
void bypass_whitespace() {
   while (isspace(*line)) {
      line++;
   }
}

/**
 * This function will determine if a provided character begins a valid lexeme. 
 * If so, a value of True will be returned. If not, an appropriate message will 
 * be printed to the output file and False will be returned.
 *
 * @param token A character to be checked for validity.
 * @param out_file A pointer to the output file.
 * @return True if the character represents a valid token. False otherwise.
 */
int isvalid(char token, FILE * out_file) {
   int result = TRUE;
   if (token == '@') {
      line--;
      fprintf(out_file, "===> '%c'\n%s", *line, ERROR);
      line++;
      result = FALSE;
   }
   return result;
}

/**
 * This function checks if the next character in line is an equals symbol. 
 * If it is, it is appended to the token array. If not, nothing happens.
 *
 * @param token_ptr A pointer to a memory location used to store a lexeme.
 */
void check_for_equals(char * token_ptr) {
   if (*line == '=') {
      token_ptr++;
      line++;
      *token_ptr = '=';
   }
}

/**
 * Appends digits to the lexeme until a non-numeric character is found.
 *
 * @param token_ptr A pointer to a memory location used to store a lexeme.
 */
void append_digits(char * token_ptr) {
   while (isdigit(*line)) {
      token_ptr++;
      *token_ptr = *line;
      line++;
   }
}
