#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

Args parse_args(int argc, char **argv)
{
    Args args = {0};
    static struct option long_options[] = {
        { "help",   no_argument,       0, 'h' },
        { "output", required_argument, 0, 'o' },
        { "var",    required_argument, 0, 'v' },
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "ho:v:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h': {
                args.usage = true;
            } break;

            case 'o': {
                args.output_file = optarg;
            } break;

            case 'v': {
                args.var_name = optarg;
            } break;

            default: {
                args.usage = true;
            } break;
        }
    }

    if (argc - optind > 1) {
        args.usage = true;
        return args;
    }

    if (optind < argc) {
        args.input_file = argv[optind];
    }

    return args;
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);

    char *result = malloc(len1 + len2 + 1);
    if (result == NULL)
        return NULL;

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

void adapt_var_name(char *str, size_t count)
{
    if (count <= 0)
        return;

    char *c = &str[0];

    if ('0' <= *c && *c <= '9') {
        *c = 'n';
    }

    for (size_t i = 0; i < count; ++i) {
        c = &str[i];

        if (!(*c == '_' ||
            ('a' <= *c && *c <= 'z') ||
            ('A' <= *c && *c <= 'Z') ||
            ('0' <= *c && *c <= '9')))
            *c = '_';
    }
}
