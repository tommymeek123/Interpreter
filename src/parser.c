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
 * Recognizer for the <bexpr> production rule.
 * The derivation is <bexpr>  ->  <expr> ;
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return The total value of the evaluated expression.
 */
int bexpr(char * token) {
   int subtotal = expr(token);
   if (strncmp(token, ";", 1)) {
      subtotal = ERROR;
   }
   return subtotal;
}

/**
 * Recognizer for the <expr> production rule.
 * The derivation is <expr>  ->  <term> <ttail>
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return A running subtotal of the expression.
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
 * Recognizer for the <term> production rule.
 * The derivation is <term>  ->  <stmt> <stail>
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return A running subtotal of the expression.
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
 * Recognizer for the <stmt> production rule.
 * The derivation is <stmt>  ->  <factor> <ftail>
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return A running subtotal of the expression.
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
 * Recognizer for the <factor> production rule.
 * The derivation is <factor>  ->  <expp> ^ <factor> | <expp>
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return A running subtotal of the expression.
 */
int factor(char * token) {
   int subtotal = expp(token);
   if (subtotal == ERROR) {
      return subtotal;
   } else if (!strncmp(token, "^", 1)) {
      expon_tok(token);
      int factor_value = factor(token);

      // if factor returned an error, give up otherwise return the power
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return power(subtotal, factor_value);
      }
   } else {
      return subtotal;
   }
}

/**
 * Recognizer for the <expp> production rule.
 * The derivation is <expp>  ->  ( <expr> ) | <num>
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return A running subtotal of the expression.
 */
int expp(char * token) {
   int subtotal;

   if (!strncmp(token, "(", 1)) {
      open_paren_tok(token);
      subtotal = expr(token);
      if (strncmp(token, ")", 1)) {
         subtotal = ERROR;
         token = ")";
      } else {
         closed_paren_tok(token);
      }
   } else {
      subtotal = num(token);
   }
   return subtotal;
}

/**
 * Recognizer for the <ttail> production rule.
 * The derivation is <ttail>  ->  <add_sub_tok> <term> <ttail> | e
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @param subtotal A running subtotal of the expression.
 * @return A running subtotal of the expression.
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
 * Recognizer for the <stail> production rule.
 * The derivation is <stail>  ->  <mult_div_tok> <stmt> <stail> | e
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @param subtotal A running subtotal of the expression.
 * @return A running subtotal of the expression.
 */
int stail(char * token, int subtotal) {
   int stmt_value;

   if (!strncmp(token, "*", 1)) {
      mul_div_tok(token);
      stmt_value = stmt(token);

      // if stmt returned an error, give up otherwise call stail
      if (stmt_value == ERROR) {
         return stmt_value;
      } else {
         return stail(token, (subtotal * stmt_value));
      }
   } else if (!strncmp(token, "/", 1)) {
      mul_div_tok(token);
      stmt_value = stmt(token);

      // if stmt returned an error, give up otherwise call stail
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
 * Recognizer for the <ftail> production rule.
 * The derivation is <ftail>  ->  <compare_tok> <factor> <ftail> | e
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @param subtotal A running subtotal of the expression.
 * @return A running subtotal of the expression.
 */
int ftail(char * token, int subtotal) {
   int factor_value;

   if (!strncmp(token, "<", 1)) {
      compare_tok(token);
      factor_value = factor(token);

      // if factor returned an error, give up otherwise call ftail
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal < factor_value));
      }
   } else if (!strncmp(token, "<=", 2)) {
      compare_tok(token);
      factor_value = factor(token);

      // if factor returned an error, give up otherwise call ftail
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal <= factor_value));
      }
   } else if (!strncmp(token, ">", 1)) {
      compare_tok(token);
      factor_value = factor(token);

      // if factor returned an error, give up otherwise call ftail
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal > factor_value));
      }
   } else if (!strncmp(token, ">=", 2)) {
      compare_tok(token);
      factor_value = factor(token);

      // if factor returned an error, give up otherwise call ftail
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal >= factor_value));
      } 
   } else if (!strncmp(token, "==", 2)) {
      compare_tok(token);
      factor_value = factor(token);

      // if factor returned an error, give up otherwise call ftail
      if (factor_value == ERROR) {
         return factor_value;
      } else {
         return ftail(token, (subtotal == factor_value));
      }
   } else if (!strncmp(token, "!=", 2)) {
      compare_tok(token);
      factor_value = factor(token);

      // if factor returned an error, give up otherwise call ftail
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
 * Recognizer for the <add_sub_tok> production rule.
 * The derivation is <add_sub_tok>  ->  + | -
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 */
void add_sub_tok(char * token) {
   get_token(token);
}


/**
 * Recognizer for the <mul_div_tok> production rule.
 * The derivation is <mul_div_tok>  ->  * | /
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 */
void mul_div_tok(char * token) {
   get_token(token);
}

/**
 * Recognizer for the <compare_tok> production rule.
 * The derivation is <compare_tok>  ->  < | > | <= | >= | != | ==
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 */
void compare_tok(char * token) {
   get_token(token);
}

/**
 * Functions similarly to the other terminal producing functions here 
 * even though <expon_tok> is not a production rule in the language.
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 */
void expon_tok(char * token) {
   get_token(token);
}

/**
 * Functions similarly to the other terminal producing functions here 
 * even though <open_paren_tok> is not a production rule in the language.
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 */
void open_paren_tok(char * token) {
   get_token(token);
}

/**
 * Functions similarly to the other terminal producing functions here 
 * even though <closed_paren_tok> is not a production rule in the language.
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 */
void closed_paren_tok(char * token) {
   get_token(token);
}

/**
 * Recognizer for the <num> production rule.
 * The derivation is <num>  ->  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 * 
 * @param token A pointer to the location where the next lexeme is stored.
 * @return The value of the number. A terminal in the language.
 */
int num(char * token) {
   int value = isdigit(*token) ? atoi(token) : ERROR;
   if (*token == INVALID_LEXEME) {
      lex_err();
   } else {
      get_token(token);
   }
   return value;
}

/**
 * Handles the case of an invalid lexeme. Makes sure the global line pointer 
 * is pointing to the invalid lexeme in question.
 */
void lex_err() {
   char * alpha_ptr = line;
   while (isalpha(*alpha_ptr)) {
      alpha_ptr++;
   }
   *alpha_ptr = '\0';
   line--;
}

/**
 * Integer exponents. Raises a base to a power.
 * 
 * @param base The base.
 * @param exp The exponent.
 * @return The result of the base raised to the power.
 */
int power(int base, int exp) {
   // .5 is added to avoid rounding errors
   return (int)(pow((double)base, (double)exp) + .5);
}