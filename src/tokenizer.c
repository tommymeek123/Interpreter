/**
 * A lexeme recognizer for the Interpreter project.
 *
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2020-03-20
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "interpreter.h"


/* Global variables */
extern char * line;          // pointer to the next character to process

/**
 * This function determines the next complete lexeme in the input stream. It 
 * then puts that token in the location pointed to by token_ptr.
 *
 * @param token_ptr A pointer to a memory location used to store a lexeme.
 */
void get_token(char * token_ptr) {
   bypass_whitespace();
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
         *token_ptr = INVALID_LEXEME;
         break;
   }
   token_ptr++;
   *token_ptr = '\0';
   bypass_whitespace();
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
   if (token == INVALID_LEXEME) {
      line--;
      fprintf(out_file, "===> '%c'\n%s", *line,"Lexical error: not a lexeme\n");
      line++;
      result = FALSE;
   }
   return result;
}

// /**
//  * This function checks if the next character in line is an equals symbol. 
//  * If it is, it is appended to the token array. If not, nothing happens.
//  *
//  * @param token_ptr A pointer to a memory location used to store a lexeme.
//  */
// void check_for_equals(char ** token_ptr) {
//    if (*line == '=') {
//       *token_ptr++;
//       line++;
//       **token_ptr = '=';
//    }
// }

// /**
//  * Appends digits to the lexeme until a non-numeric character is found.
//  *
//  * @param token_ptr A pointer to a memory location used to store a lexeme.
//  */
// void append_digits(char ** token_ptr) {
//    while (isdigit(*line)) {
//       *token_ptr++;
//       **token_ptr = *line;
//       line++;
//    }
// }
