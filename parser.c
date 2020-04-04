/**
 * A recursive descent parser for a simple expression language. Most of the 
 * functions in this file model a non-terminal in the grammar listed below.
 *
 * @author Mark Holliday
 * @author William Kreahling
 * created on 2008-09-26
 * modified on 2015-03-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "tokenizer.h"
#include "parser.h"

/*
 * <bexpr> ::= <expr> ;
 * <expr> ::=  <term> <ttail>
 * <ttail> ::=  <add_sub_tok> <term> <ttail> | e
 * <term> ::=  <stmt> <stail>
 * <stail> ::=  <mult_div_tok> <stmt> <stail> | e
 * <stmt> ::=  <factor> <ftail>
 * <ftail> ::=  <compare_tok> <factor> <ftail> | e
 * <factor> ::=  <expp> ^ <factor> | <expp>
 * <expp> ::=  ( <expr> ) | <num>
 * <add_sub_tok> ::=  + | -
 * <mul_div_tok> ::=  * | /
 * <compare_tok> ::=  < | > | <= | >= | ! = | ==
 * <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 */


// <expr> -> <term> <ttail>
int expr(char *token) {
   int subtotal = term(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else {
      return ttail(token, subtotal);
   }
}

// <ttail> -> <add_sub_tok> <term> <ttail> | e
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
