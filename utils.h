#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    bool usage;
    const char *input_file;
    char *output_file;
    char *var_name;
} Args;

Args parse_args(int argc, char **argv);

char* concat(const char *s1, const char *s2);

void adapt_var_name(char *str, size_t count);

#endif // _UTILS_H_
