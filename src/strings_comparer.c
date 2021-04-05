#include <stdio.h>
#include <stdlib.h>
#include "sortings.h"

#define MAX_FILENAME_LEN 100
#define VALID_ARGUMENTS_NUM 6

#define ERROR_CODE -1

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef void (*sort_func_t)(strings_array_t, array_size_t, comparator_func_t);

typedef struct {
    int number_of_string;
    char input_filename[MAX_FILENAME_LEN];
    char output_filename[MAX_FILENAME_LEN];
    sort_func_t sort_func;
    comparator_func_t comparer_type;
} arguments_t;

int is_a_number(const char *string) {
    for (int i = 0; ; i++) {
        if (string[i] == 0)
            return 1;
        if (string[i] < '0' || string[i] > '9')
            return 0;
    }
}

int asc_comparer_type(const char *str1, const char *str2) {
    return strcmp(str1, str2);
}

int des_comparer_type(const char *str1, const char *str2) {
    return -strcmp(str1, str2);
}

int getting_the_arguments(int argc, char **argv, arguments_t *arguments) {
    if (argc != VALID_ARGUMENTS_NUM) {
        error("Wrong number of parameters\n");
        return ERROR_CODE;
    }
    if (!is_a_number(argv[1])) {
        error("First argument must be number\n");
        return ERROR_CODE;
    }
    arguments->number_of_string = (int)strtoll(argv[1], NULL, 10);
    if (arguments->number_of_string < 0) {
        error("Number of strings must be greater than 0\n");
        return ERROR_CODE;
    }
    strncpy(arguments->input_filename, argv[2], MAX_FILENAME_LEN);
    strncpy(arguments->output_filename, argv[3], MAX_FILENAME_LEN);
    if (strcmp(argv[4], "bubble") == 0) {
        arguments->sort_func = bubble;
    } else if (strcmp(argv[4], "insertion") == 0) {
        arguments->sort_func = insertion;
    } else if (strcmp(argv[4], "merge") == 0) {
        arguments->sort_func = merge;
    } else if (strcmp(argv[4], "quick") == 0) {
        arguments->sort_func = quick;
    } else if (strcmp(argv[4], "radix") == 0) {
        arguments->sort_func = radix;
    } else {
        error("You should enter valid name of sort\n");
        return ERROR_CODE;
    }
    if (strcmp(argv[5], "asc") == 0) {
        arguments->comparer_type = asc_comparer_type;
    } else if (strcmp(argv[5], "des") == 0) {
        arguments->comparer_type = des_comparer_type;
    } else {
        error("You should enter valid name of comparator\n");
        return ERROR_CODE;
    }
    return 0;
}

void clear_memory(strings_array_t *array, array_size_t size) {
    for (array_size_t i = 0; i < size; ++i) {
        free((*array)[i]);
    }
    free(*array);
}

int allocate_memory(strings_array_t *array, int number_of_string) {
    (*array) = malloc(sizeof(char *) * number_of_string);
    if ((*array) == NULL) {
        error("Cannot allocate array\n");
        return ERROR_CODE;
    }
    for (int i = 0; i < number_of_string; ++i) {
        (*array)[i] = malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);
        if ((*array)[i] == NULL) {
            error("Cannot allocate array[%d]\n", i);
            clear_memory(array, i);
        }
    }
    return 0;
}

int reading_strings(const char *filename, strings_array_t strings, int number_of_strings) {
    FILE *input = fopen(filename, "rt");
    if (input == NULL) {
        error("Cannot open file %s", filename);
        return ERROR_CODE;
    }
    for (int i = 0; i < number_of_strings; ++i) {
        if (fgets(strings[i], MAX_INPUT_STRING_SIZE, input) == NULL) {
            error("Error of reading from %s", filename);
            return ERROR_CODE;
        }
    }
    char *last_string = strings[number_of_strings - 1];
    size_t last_string_len = strlen(last_string);
    if (last_string[last_string_len - 1] != '\n') {
        last_string[last_string_len] = '\n';
        last_string[last_string_len + 1] = '\0';
    }
    fclose(input);
    return 0;
}

int writing_strings(const char *filename, strings_array_t strings, int number_of_strings) {
    FILE *output = fopen(filename, "wt");
    if (output == NULL) {
        error("Cannot open file %s", filename);
        return ERROR_CODE;
    }
    for (int i = 0; i < number_of_strings; ++i) {
        if (fputs(strings[i], output) == -1) {
            error("Error of writing to %s", filename);
            return ERROR_CODE;
        }
    }
    fclose(output);
    return 0;
}

int main(int argc, char *argv[]) {
    arguments_t arguments;
    int setting_result = getting_the_arguments(argc, argv, &arguments);
    if (setting_result != 0) {
        return setting_result;
    }
    if (arguments.number_of_string == 0) {
        char *n[1] = {"\n"};
        int writing_result = writing_strings(arguments.output_filename, n, 1);
        if (writing_result != 0) {
            error("Cannot write \\n to file %s\n", arguments.output_filename);
            return ERROR_CODE;
        }
        return 0;
    }
    strings_array_t strings = NULL;
    int allocation_result = allocate_memory(&strings, arguments.number_of_string);
    if (allocation_result != 0) {
        return allocation_result;
    }
    int reading_result = reading_strings(arguments.input_filename, strings, arguments.number_of_string);
    if (reading_result != 0) {
        return reading_result;
    }
    arguments.sort_func(strings, arguments.number_of_string, arguments.comparer_type);
    int writing_result = writing_strings(arguments.output_filename, strings, arguments.number_of_string);
    if (writing_result != 0) {
        return writing_result;
    }
    clear_memory(&strings, arguments.number_of_string);
    return 0;
}