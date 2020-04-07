/**
 * Header file for tokenizer.c. Named constant definitions and 
 * funtion prototypes are included.
 *
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2020-03-20
 */

/* Constants */
#define LINE 100
#define TSIZE 20
#define TRUE 1
#define FALSE 0
#define DASHES "---------------------------------------------------------\n"
#define ERROR "Lexical error: not a lexeme\n"

/* Function prototypes */
void get_token(char *);
void bypass_whitespace();
int isvalid(char, FILE *);
void check_for_equals(char *);    // Not used
void append_digits(char *);       // Not used
