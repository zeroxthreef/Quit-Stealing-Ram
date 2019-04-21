#ifndef _CPARSER_H
#define _CPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* it's 2019, if you don't have a compiler with c99 as your system compiler what are you doing */
#include <stdbool.h>

int config_parse_file(FILE* file);

// gets value
char* config_get_item(const char* value_name);

#endif

