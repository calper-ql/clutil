#include "cclog.h"

unsigned short _cclog_spaces = 0;

unsigned short cclog_zero_space() {
    _cclog_spaces = 0;
    return _cclog_spaces;
}

unsigned short cclog_space_count() {
    return _cclog_spaces;
}

unsigned short cclog_add_space() {
    if (_cclog_spaces < 65535) _cclog_spaces++;
    return _cclog_spaces;
}

unsigned short cclog_remove_space() {
    if (_cclog_spaces > 0) _cclog_spaces--;
    return _cclog_spaces;
}

void cclog_erase_line() {
    printf(CLOG_ANSI_ERASE_LINE);
    fflush(stdout);
}

void cclog_print_spaces() {
    char sp[_cclog_spaces + 1];
    for (unsigned i = 0; i < _cclog_spaces; i++) sp[i] = ' ';
    sp[_cclog_spaces] = '\0';
    printf("%s", sp);
}

void cclog_print(std::string str) {
    cclog_print_spaces();
    std::cout << str << std::endl;
}

std::string cclog_wrap(std::string str, std::string ansi_color) {
    std::string out;
    out = ansi_color + str + CLOG_ANSI_DEFAULT;
    return out;
}

void cclog_raw_log(std::string message, std::string interior, std::string ansi_color) {
    std::string out;
    out = "[" + cclog_wrap(interior, ansi_color) + "] - " + message;
    cclog_erase_line();
    cclog_print_spaces();
    std::cout << out << std::flush;
}

void cclog_wait(std::string message, std::string status) {
    cclog_raw_log(message, status, CLOG_ANSI_BLUE);
}

void cclog_success(std::string message) {
    cclog_raw_log(message, "SUCCESS", CLOG_ANSI_GREEN);
    std::cout << std::endl;
}

void cclog_failure(std::string message) {
    cclog_raw_log(message, "FAILURE", CLOG_ANSI_RED);
    std::cout << std::endl;
}

void cclog_bar(std::string prefix, std::string suffix, float percentage) {
    float converted = percentage;
    if (percentage < 0.0) converted = 0.0;
    if (percentage > 100.0) converted = 100.0;
    int count = 20;
    cclog_erase_line();
    cclog_print_spaces();
    printf("%s[", prefix.c_str());
    for (int i = 0; i < count; i++) {
        if (i / (float) count < converted / 100.0) printf("\u2588");
        else printf(" ");
    }
    printf("]-[%.2f]%s", converted, suffix.c_str());
    fflush(stdout);
}

void cclog_bar_finish(std::string prefix, std::string suffix) {
    cclog_bar(prefix, suffix, 100.0);
    printf("\n");
}