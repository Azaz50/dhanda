#ifndef _DHANDA_UTIL_H_
#define _DHANDA_UTIL_H_
#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <dhanda/sqlite3.h>



#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_WHITE    "\x1b[37m"
#define ANSI_RESET          "\x1b[0m"

#define ANSI_BG_RED         "\x1b[41m"
#define ANSI_BG_GREEN       "\x1b[42m"
#define ANSI_BG_YELLOW      "\x1b[43m"

#define ANSI_BLINK          "\x1b[5m"
#define ANSI_BOLD           "\x1b[1m"

#define start_cmdline_color() printf(ANSI_BOLD ANSI_COLOR_CYAN);
#define end_cmdline_color()   printf(ANSI_RESET); fflush(stdout);

#if defined(DEBUG)
#define debug_print(fmt) fprintf(stderr, "%s:%s:%d: %s\n", __FILE__, __func__, __LINE__, fmt);
#else
#define debug_print(fmt)
#endif

int get_line(char line[], int size);

int get_string(char line[], int size);

void input_valid_string(char *input, size_t size, int (*validator)(char *));
int validate_name(char *str);

int validate_desc(char *str);

void input_phone(char *input, int (*validator)(char *));
int validate_phone(char *str);

void input_amount(int *input, int (*validator)(char *));
int validate_amount(char *str);
int validate_amount_kore(int amount);

void title_case(char *str);

void created_at(time_t t);
void updated_at(time_t t);

void input_pid(dhanda *app, int *pid, int (*validator)(char *));
int validate_pid(char *str);

void unispace_separate(char *str);

void input_txn_type(char *type, int (*validator) (char *));
int validate_type(char *str);

int print_user(void *not_used, int ncols, char **values, char **fields);

void * updated_time(time_t t);
void * created_time(time_t t);

time_t unix_time(char *timestr);

int 
cb_party_list(void *list, int ncols, char **values, char **fields);
int
cb_party_struct(void *ptr, int ncols, char **values, char **fields);

int put_in_txn_struct(void *ptr, int ncols, char **values, char **fields);
int put_in_txn_list(void *ptr, int ncols, char **values, char **fields);





void sky();
void red();
void yellow();
void reset ();
void grey();
void bold();
void warning();
void error();
void success();
void print_success(char *s);
void print_error(char *s);
void print_warning(char *s);

void centreprint(char *s);

#endif
