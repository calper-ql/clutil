#include "clog.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short _clog_spaces = 0;

unsigned short clog_zero_space() {
    _clog_spaces = 0;
    return _clog_spaces;
}

unsigned short clog_space_count() {
    return _clog_spaces;
}

unsigned short clog_add_space() {
    if (_clog_spaces < 65535) _clog_spaces++;
    return _clog_spaces;
}

unsigned short clog_remove_space() {
    if (_clog_spaces > 0) _clog_spaces--;
    return _clog_spaces;
}

void clog_erase_line() {
    printf(CLOG_ANSI_ERASE_LINE);
    fflush(stdout);
}

void clog_print_spaces() {
    char* sp = (char*) malloc(_clog_spaces+2);
    for (unsigned i = 0; i < _clog_spaces; i++) sp[i] = ' ';
    sp[_clog_spaces] = '\0';
    printf("%s", sp);
	free(sp);
}

void clog_print(char *str) {
    clog_print_spaces();
    printf("%s\n", str);
}

char *clog_wrap(char *str, const char *ansi_color) {
    size_t color_len = strlen(ansi_color);
    size_t str_len = strlen(str);
    size_t default_len = strlen(CLOG_ANSI_DEFAULT);
    char *allocated = (char *) malloc(color_len + str_len + default_len + 1);
    sprintf(allocated, "%s%s%s", ansi_color, str, CLOG_ANSI_DEFAULT);
    return allocated;
}

void clog_raw_log(char *message, char *interior, const char *ansi_color) {
    clog_erase_line();
    clog_print_spaces();
    char *colored = clog_wrap(interior, ansi_color);
    printf("[%s] - %s", colored, message);
    fflush(stdout);
    free(colored);
}

void clog_wait(char *message, char *status) {
    return clog_raw_log(message, status, CLOG_ANSI_BLUE);
}

void clog_success(char *message) {
    clog_raw_log(message, "SUCCESS", CLOG_ANSI_GREEN);
    printf("\n");
}

void clog_failure(char *message) {
    clog_raw_log(message, "FAILURE", CLOG_ANSI_RED);
    printf("\n");
}

void clog_bar(char *prefix, char *suffix, float percentage) {
    float converted = percentage;
    if (percentage < 0.0) converted = 0.0;
    if (percentage > 100.0) converted = 100.0;
    int count = 20;
    clog_erase_line();
    clog_print_spaces();
    printf("%s[", prefix);
    for (int i = 0; i < count; i++) {
        if (i / (float) count < converted / 100.0) printf("\u2588");
        else printf(" ");
    }
    printf("]-[%.2f]%s", converted, suffix);
    fflush(stdout);
}

void clog_bar_finish(char *prefix, char *suffix) {
    clog_bar(prefix, suffix, 100.0);
    printf("\n");
}
