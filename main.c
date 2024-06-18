#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

void print_usage(void)
{
    static const char *usage =
        "Usage: cembed [OPTIONS] FILE\n"
        "\n"
        "  -h, --help     Print this help message\n"
        "  -o, --output   Output file path\n"
        "  -v, --var      Name of variable in header file";

    printf("%s\n", usage);
}

bool dump_file(const char *input_file_path, const char *output_file_path, const char* var_name)
{
    FILE *input_file = fopen(input_file_path, "rb");
    if (input_file == NULL)
        return false;

    FILE *output_file = fopen(output_file_path, "wb");
    if (output_file == NULL)
        return false;

    fseek(input_file, 0L, SEEK_END);
    size_t size = ftell(input_file);
    rewind(input_file);

    fprintf(output_file, "static unsigned char %s[%ld]={\n", var_name, size);

    uint8_t byte;
    for (size_t i = 0; i < size - 1; ++i) {
        fread(&byte, 1, 1, input_file);
        fprintf(output_file, "0x%02X,", byte);
        if ((i + 1) % 16 == 0)
            fprintf(output_file, "\n");
    }
    fread(&byte, 1, 1, input_file);
    fprintf(output_file, "0x%02X", byte);

    fprintf(output_file, "};\n");

    fclose(input_file);
    fclose(output_file);

    return true;
}

int main(int argc, char **argv)
{
    Args args = parse_args(argc, argv);

    if (args.usage) {
        print_usage();
        return EXIT_SUCCESS;
    }

    if (args.input_file == NULL) {
        fprintf(stderr, "ERROR: no input file provided\n");
        print_usage();
        return EXIT_FAILURE;
    }

    if (args.output_file == NULL) {
        args.output_file = concat(args.input_file, ".h");
        if (args.output_file == NULL) {
            fprintf(stderr, "ERROR: Could not allocate memory for output file string\n");
            return EXIT_FAILURE;
        }
    }

    if (args.var_name == NULL) {
        size_t in_len = strlen(args.input_file);
        args.var_name = malloc(in_len + 1);
        if (args.var_name == NULL) {
            fprintf(stderr, "ERROR: Could not allocate memory for var name string\n");
            return EXIT_FAILURE;
        }
        memcpy(args.var_name, args.input_file, in_len + 1);
    }

    adapt_var_name(args.var_name, strlen(args.var_name));

    if (!dump_file(args.input_file, args.output_file, args.var_name)) {
        fprintf(stderr, "ERROR: could not generate %s\n", args.output_file);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
