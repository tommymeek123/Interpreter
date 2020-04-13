/**
 * Header file for the Interpreter project. Named constant definitions and 
 * funtion prototypes are included.
 *
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2020-04-13
 */

/* Constants */
#define LSIZE 100
#define TSIZE 20
#define TRUE 1
#define FALSE 0
#define DASHES "---------------------------------------------------------\n"
#define INVALID_LEXEME '@'
#define ERROR -999999
#define EOL_ERROR '~'

/* Function prototypes */
// tokenizer.c
void get_token(char *);
void bypass_whitespace();
int isvalid(char, FILE *);

// interpreter.c
void usage(int);
FILE ** open_files(char **);
void close_files(FILE **);
void tokenize(FILE *, FILE *);
void parse(FILE *, FILE *);

// parser.c
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
int power(int, int);              // helper function
void lex_err();                   // helper function