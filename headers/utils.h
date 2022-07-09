#ifndef __UTILS_H__
#define __UTILS_H__

#include <string.h>
#include <ctype.h>

int find_pattern(char* str, char* pat);
int count_pattern(char* str, char* pat);
char* to_lower_case(char* s);
char* to_upper_case(char* s);

#endif