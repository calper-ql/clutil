#ifndef CLOG_H_
#define CLOG_H_

#include <unistd.h>
#include "ansi.h"

extern unsigned short _clog_spaces;

unsigned short clog_zero_space();

unsigned short clog_space_count();

unsigned short clog_add_space();

unsigned short clog_remove_space();

void clog_erase_line();

void clog_print_spaces();

void clog_print(char *str);

char *clog_wrap(char *str, const char *ansi_color);

void clog_raw_log(char *message, char *interior, const char *ansi_color);

void clog_wait(char *message, char *status);

void clog_success(char *message);

void clog_failure(char *message);

void clog_bar(char *prefix, char *suffix, float percentage);

void clog_bar_finish(char *prefix, char *suffix);

#endif
