/**
 * A recursive descent parser for a simple expression language. Most of the 
 * functions in this file model a non-terminal in the grammar listed below.
 *
 * @author Mark Holliday
 * @author William Kreahling
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2008-09-26
 * modified on 2015-03-25
 * modified on 2020-04-13
 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

/*
 * <bexpr>       ->  <expr> ;
 * <expr>        ->  <term> <ttail>
 * <ttail>       ->  <add_sub_tok> <term> <ttail> | e
 * <term>        ->  <stmt> <stail>
 * <stail>       ->  <mult_div_tok> <stmt> <stail> | e
 * <stmt>        ->  <factor> <ftail>
 * <ftail>       ->  <compare_tok> <factor> <ftail> | e
 * <factor>      ->  <expp> ^ <factor> | <expp>
 * <expp>        ->  ( <expr> ) | <num>
 * <add_sub_tok> ->  + | -
 * <mul_div_tok> ->  * | /
 * <compare_tok> ->  < | > | <= | >= | != | ==
 * <num>         ->  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 */

/* Global variables */
extern char * line;

/**
* TODO: DESCRIPTION. <bexpr>  ->  <expr> ;
* 
* @param token TODO: DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int bexpr(char * token) {
   int subtotal = expr(token);
   get_token(token);
   if (token != ';') {
      subtotal = ERROR;
   }
   return subtotal;
}

/**
* TODO: DESCRIPTION. <expr>  ->  <term> <ttail>
* 
* @param token TODO: DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int expr(char * token) {
   int subtotal = term(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return ttail(token, subtotal);
   }
}

/**
* TODO: DESCRIPTION. <term>  ->  <stmt> <stail>
* 
* @param token TODO: DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int term(char * token) {
   int subtotal = stmt(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return stail(token, subtotal);
   }
}

/**
* TODO: DESCRIPTION. <stmt>  ->  <factor> <ftail>
* 
* @param token TODO: DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int stmt(char * token) {
   int subtotal = factor(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return ftail(token, subtotal);
   }
}

/**
* TODO: DESCRIPTION. <factor>  ->  <expp> ^ <factor> | <expp>
* 
* @param token TODO: DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int factor(char * token) {
   int subtotal = expp(token);
   int factor_value;
   if (!strncmp(token, "^", 1)) {
      expon_tok(token);
      factor_value = factor(token);

      // if term returned an error, give up otherwise call ttail
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return subtotal ^ factor_value;
      }
   } else {
      return subtotal;
   }
}

/**
* TODO: DESCRIPTION. <expp>  ->  ( <expr> ) | <num>
* 
* @param token TODO: DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int expp(char * token) {
   if (!strncmp(token, "(", 1)) {
      return expr(token);
   }
   num(token); //not right
   return token; //this either
}

/**
* TODO: DESCRIPTION. <ttail>  ->  <add_sub_tok> <term> <ttail> | e
* 
* @param token TODO: DESCRIPTION.
* @param subtotal DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int ttail(char * token, int subtotal) {
   int term_value;

   if (!strncmp(token, "+", 1)) {
      add_sub_tok(token);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR) {
         return term_value;
      } else {
         return ttail(token, (subtotal + term_value));
      }
   } else if (!strncmp(token, "-", 1)) {
      add_sub_tok(token);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR) {
         return term_value;
      } else {
         return ttail(token, (subtotal - term_value));
      }
   } else {
      // empty string
      return subtotal;
   }
}

/**
* TODO: DESCRIPTION. <stail>  ->  <mult_div_tok> <stmt> <stail> | e
* 
* @param token TODO: DESCRIPTION.
* @param subtotal DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int stail(char * token, int subtotal) {
   int stmt_value;

   if (!strncmp(token, "*", 1)) {
      mul_div_tok(token);
      stmt_value = stmt(token);

      if (stmt_value == ERROR) {
         return stmt_value;
      } else {
         return stail(token, (subtotal * stmt_value));
      }
   } else if (!strncmp(token, "/", 1)) {
      mul_div_tok(token);
      stmt_value = stmt(token);
      if (stmt_value == ERROR) {
         return stmt_value;
      } else {
         return stail(token, (subtotal / stmt_value));
      }
   } else {
      // empty string
      return subtotal;
   }
}

/**
* TODO: DESCRIPTION. <ftail>  ->  <compare_tok> <factor> <ftail> | e
* 
* @param token TODO: DESCRIPTION.
* @param subtotal DESCRIPTION.
* @return TODO: DESCRIPTION.
*/
int ftail(char * token, int subtotal) {
   int factor_value;

   if (!strncmp(token, "<", 1)) {
      compar_tok(token);
      factor_value = factor(token);

      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal < factor_value));
      }
   } else if (!strncmp(token, "<=", 2)) {
      compar_tok(token);
      factor_value = factor(token);

      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal <= factor_value));
      }
   } else if (!strncmp(token, ">", 1)) {
      compar_tok(token);
      factor_value = factor(token);

      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal > factor_value));
      }
   } else if (!strncmp(token, ">=", 2)) {
      compar_tok(token);
      factor_value = factor(token);

      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal >= factor_value));
      } 
   } else if (!strncmp(token, "==", 2)) {
      compar_tok(token);
      factor_value = factor(token);

      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal == factor_value));
      }
   } else if (!strncmp(token, "!=", 2)) {
      compar_tok(token);
      factor_value = factor(token);

      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal != factor_value));
      } 
   } else {
      // empty string
      return subtotal;
   }
}

/**
* TODO: DESCRIPTION. <add_sub_tok>  ->  + | -
* 
* @param token TODO: DESCRIPTION.
*/
void add_sub_tok(char * token) {
   get_token(token);
}

/**
* TODO: DESCRIPTION. <mul_div_tok>  ->  * | /
* 
* @param token TODO: DESCRIPTION.
*/
void mul_div_tok(char * token) {
   get_token(token);
}

/**
* TODO: DESCRIPTION. <compare_tok>  ->  < | > | <= | >= | != | ==
* 
* @param token TODO: DESCRIPTION.
*/
void compare_tok(char * token) {
   get_token(token);
}

/**
* TODO: DESCRIPTION. <num>  ->  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
* 
* @param token TODO: DESCRIPTION.
*/
int num(char * token) {
   get_token(token);
}

/**
*A method to convert a char into an int. 
Logic behind the calculation above is to play with ASCII values. 
ASCII value of character 8 is 56, ASCII value of character 0 is 48. 
ASCII value of integer 8 is 8.
*/
int char_to_digit(char * character){
   //not sure if this needs to be char or *char. I think *char because I want the 
   //value that is storied in the token pointer. 
   int int_of_char = *character - '0';
   return int_of_char;
}