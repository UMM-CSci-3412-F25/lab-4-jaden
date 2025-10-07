#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 1024

bool is_vowel(char c) {
    char lower_c = (char)tolower(c);
    return (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || lower_c == 'o' || lower_c == 'u');
}

int copy_non_vowels(int num_chars, char* in_buf, char* out_buf) {
    int out_index = 0;
    for (int i = 0; i < num_chars; i++) {
        if (!is_vowel(in_buf[i])) {
            out_buf[out_index] = in_buf[i];
            out_index++;
        }
    }
    return out_index;
}

void disemvowel(FILE* inputFile, FILE* outputFile) {
    char in_buf[BUF_SIZE];
    char out_buf[BUF_SIZE];
    size_t chars_read;
    int chars_to_write;

    // Use fread to repeatedly read chunks of data
    while ((chars_read = fread(in_buf, 1, BUF_SIZE, inputFile)) > 0) {
        // Copy non-vowels to the output buffer
        chars_to_write = copy_non_vowels((int)chars_read, in_buf, out_buf);

        // Write the disemvoweled data to the output file
        fwrite(out_buf, 1, chars_to_write, outputFile);
    }
}

int main(int argc, char *argv[]) {
    // Default to stdin and stdout
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    bool must_close_input = false;
    bool must_close_output = false;

    // Process command line arguments
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [input_file] [output_file]\n", argv[0]);
        return 1; // Error exit code
    }

    if (argc >= 2) {
        // One or two arguments: the first is the input file
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            perror("Error opening input file");
            return 2; // Error exit code
        }
        must_close_input = true;
    }

    if (argc == 3) {
        outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {
            if (must_close_input) {
                fclose(inputFile);
            }
            perror("Error opening output file");
            return 3; // Error exit code
        }
        must_close_output = true;
    }

    disemvowel(inputFile, outputFile);

    // Clean up
    if (must_close_input) {
        fclose(inputFile);
    }
    if (must_close_output) {
        fclose(outputFile);
    }

    return 0;
}