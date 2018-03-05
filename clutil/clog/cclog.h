#ifndef CCLOG_H_
#define CCLOG_H_

#include <iostream>
#include "ansi.h"

extern unsigned short _cclog_spaces;

unsigned short cclog_zero_space();

unsigned short cclog_space_count();

unsigned short cclog_add_space();

unsigned short cclog_remove_space();

void cclog_erase_line();

void cclog_print_spaces();

void cclog_print(std::string str);

std::string cclog_wrap(std::string str, std::string ansi_color);

void cclog_raw_log(std::string message, std::string interior, std::string ansi_color);

void cclog_wait(std::string message, std::string status);

void cclog_success(std::string message);

void cclog_failure(std::string message);

void cclog_bar(std::string prefix, std::string suffix, float percentage);

void cclog_bar_finish(std::string prefix, std::string suffix);

#endif
