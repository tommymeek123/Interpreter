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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
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
 * <bexpr>  ->  <expr> ; TODO: DESCRIPTION.
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
 * <expr>  ->  <term> <ttail> TODO: DESCRIPTION.
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
 * <ttail>  ->  <add_sub_tok> <term> <ttail> | e TODO: DESCRIPTION.
 *
 * @param token TODO: DESCRIPTION.
 * @param subtotal TODO: DESCRIPTION.
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

   /**
    * <add_sub_tok>  ->  + | - TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
   void add_sub_tok(char * token) {
      get_token(token);
   }

   /**
    * <mul_div_tok>  ->  * | / TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
   void mul_div_tok(char * token) {
      get_token(token);
   }

   /**
    * <compare_tok>  ->  < | > | <= | >= | != | == TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
   void compare_tok(char * token) {
      get_token(token);
   }

   /**
    * <num>  ->  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+ TODO: DESCRIPTION.
    *
    * @param TODO: DESCRIPTION
    * @param TODO: DESCRIPTION
    */
   void num(char * token) {
      get_token(token);
   }
}