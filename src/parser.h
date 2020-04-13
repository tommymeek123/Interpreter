/**
 * Header file for parser.c. Named constant definitions and 
 * funtion prototypes are included.
 *
 * @author Mark Holliday
 * @author William Kreahling
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2008-09-26
 * modified on 2015-03-25
 * modified on 2020-04-13
 */

/* Constants */
#define ERROR -999999

/* Function prototypes */
int bexpr(char *);                // bexpr is short for boolean_expression
int expr(char *);                 // expr is short for expression
int term(char *);
int ttail(char *, int);           // ttail is short for term_tail
int stmt(char *);
int stail(char *, int);           // stail is short for statement_tail
int factor(char *);
int ftail(char *, int);	          // ftail is short for factor_tail
int expp(char *);                 // expp is short for exponentiation
void add_sub_tok(char *);
void mul_div_tok(char *);
void compare_tok(char *);
void expon_tok(char *);           // helper function
void open_paren_tok(char *);      // helper function
void closed_paren_tok(char *);    // helper function
int num(char *);
void lex_err();                   // helper function
void syn_err();                   // helper function
int power(int, int);              // helper function