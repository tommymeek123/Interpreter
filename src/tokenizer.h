/**
 * Header file for tokenizer.c. Named constant definitions and 
 * funtion prototypes are included.
 *
 * @author Justin Clifton
 * @author Tommy Meek
 * created on 2020-03-20
 */

/* Constants */
#define LSIZE 100
#define TSIZE 20
#define TRUE 1
#define FALSE 0
#define INVALID_LEXEME '@'
#define EOL_ERROR '~'
#define LEX_ERR_CH "===> '%c'\nLexical error: not a lexeme\n"

/* Function prototypes */
void get_token(char *);
void bypass_whitespace();
int isvalid(char, FILE *);
