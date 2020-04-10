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

#include "interpreter.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * <bexpr>       ->  <expr> ; x
 * <expr>        ->  <term> <ttail> x
 * <ttail>       ->  <add_sub_tok> <term> <ttail> | e x
 * <term>        ->  <stmt> <stail> x
 * <stail>       ->  <mult_div_tok> <stmt> <stail> | e x
 * <stmt>        ->  <factor> <ftail> x
 * <ftail>       ->  <compare_tok> <factor> <ftail> | e x
 * <factor>      ->  <expp> ^ <factor> | <expp>
 * <expp>        ->  ( <expr> ) | <num>
 * <add_sub_tok> ->  + | - x
 * <mul_div_tok> ->  * | / x
 * <compare_tok> ->  < | > | <= | >= | != | == x
 * <num>         ->  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+ x
 */

/* Global variables */
extern char *line;

/**
 * <bexpr>  ->  <expr> ; TODO: DESCRIPTION.
 *
 * @param token TODO: DESCRIPTION.
 * @return TODO: DESCRIPTION.
 */
int bexpr(char *token) {
   int subtotal = expr(token);
   get_token(token);
   if (token != ';') {
      subtotal = ERROR;
   }
   return subtotal;
}

/**
 * <expr>  ->  <term> <ttail> TODO: DESCRIPTION.
 *
 * @param token TODO: DESCRIPTION.
 * @return TODO: DESCRIPTION.
 */
int expr(char *token) {
   int subtotal = term(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return ttail(token, subtotal);
   }
}

/**
 * <ttail>  ->  <add_sub_tok> <term> <ttail> | e TODO: DESCRIPTION.
 *
 * @param token TODO: DESCRIPTION.
 * @param subtotal TODO: DESCRIPTION.
 * @return TODO: DESCRIPTION.
 */
int ttail(char *token, int subtotal) {
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
    * 
    *  <term> ->  <stmt> <stail> TODO: DESCRIPTION.
    * @param TODO: DESCRIPTION.
    */
int term(char *token) {
   int subtotal = stmt(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return stail(token, subtotal);
   }
}

/**
 * TODO: DESCRIPTION
 * @param TODO: DESCRIPTION.
 *  <stmt>        ->  <factor> <ftail>
 */
int stmt(char *token) {
   int subtotal = factor(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return ftail(token, subtotal);
   }
}

/**
 * TODO: DESCRIPTION.
 * <expp>        -> ( <expr> ) | <num>
 */
int expp(char * token){
   
   if(!strncmp(token,"(",1)){
      expr(token);
   }else{
      num(token);
      return token;
   }
}

/**
    * 
    * <stail>       ->  <mult_div_tok> <stmt> <stail> | e
    */
int stail(char *token, int subtotal) {
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
 * <ftail>       ->  <compare_tok> <factor> <ftail> | e
 * 
 */
int ftail(char *token, int subtotal) {
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
   }else if (!strncmp(token, "==", 2)) {
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
    * <add_sub_tok>  ->  + | - TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
void add_sub_tok(char *token) {
   get_token(token);
}

/**
    * <mul_div_tok>  ->  * | / TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
void mul_div_tok(char *token) {
   get_token(token);
}

/**
    * <compare_tok>  ->  < | > | <= | >= | != | == TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
void compare_tok(char *token) {
   get_token(token);
}

/**
    * <num>  ->  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+ TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
void num(char *token) {
   get_token(token);
}