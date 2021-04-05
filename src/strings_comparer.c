#include<stdlib.h>
#include<stdio.h>
#include "sortings.h"
#define NUMBER_OF_ARGS 6
#define MAX_FILENAME_LENGTH 100

typedef void (*sort_function)(strings_array_t, array_size_t, comparator_func_t);

int asc_comparer_type(const char *string_1, const char *string_2) {
    return strcmp(string_1, string_2);
}
int des_comparer_type(const char *string_1, const char *string_2) {
    return -strcmp(string_1, string_2);
}

typedef struct {
    int number_of_strings;
    char input_filename[MAX_FILENAME_LENGTH];
    char output_filename[MAX_FILENAME_LENGTH];
    sort_function sort_func;
    comparator_func_t comparer_type;
} input_arguments;

int is_a_number(const char *array) {
    for (int i = 0; ; i++) {
        if (array[i] == 0)
            return 1;
        if (array[i] < '0' || array[i] > '9')
            return 0;
    }
}

int getting_the_arguments(int argc, char **argv, input_arguments *arguments) {
    if (argc != NUMBER_OF_ARGS) {
        printf("Incorrect number of parameters ");
        return -1;
    }
    if (!is_a_number(argv[1])) {
        printf("First parameter must be a positive number ");
        return -1;
    }

    arguments->number_of_strings = (int) strtoll(argv[1], NULL, 10);
    if (arguments->number_of_strings < 0) {
        printf("First parameter must be a positive number ");
        return -1;
    }
    strncpy(arguments->input_filename, argv[2], MAX_FILENAME_LENGTH);
    strncpy(arguments->output_filename, argv[3], MAX_FILENAME_LENGTH);
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
        printf("Incorrect sorting name ");
        return -1;
    }
    if (strcmp(argv[5], "asc") == 0) {
        arguments->comparer_type = asc_comparer_type;
    } else if (strcmp(argv[5], "des") == 0) {
        arguments->comparer_type = des_comparer_type;
    } else {
        printf("Incorrect comparer type ");
        return -1;
    }
    return 0;
}

void clear_memory(strings_array_t *array, array_size_t array_size) {
    for(array_size_t i = 0; i < array_size; i++) {
        free((*array)[i]);
    }
    free(*array);
}

int allocate_memory(strings_array_t *array, int number_of_strings) {
    if(((*array) = malloc(sizeof(char *) * number_of_strings)) == NULL) {
        printf("Cannot allocate memory ");
        return -1;
    }
    for (int i = 0; i < number_of_strings; i++) {
        if (((*array)[i] = malloc(sizeof(char) * MAX_INPUT_STRING_SIZE)) == NULL) {
            printf("Cannot allocate memory ");
            clear_memory(array, i);
            return -1;
        }
    }
    return 0;
}

int reading_strings(const char *filename, strings_array_t array, int number_of_strings) {
    FILE *fp;
    if((fp = fopen(filename, "rt")) == NULL) {
        printf("File was not opened ");
        return -1;
    }
    for(int i = 0; i < number_of_strings; i++) {
        if((fgets(array[i], MAX_INPUT_STRING_SIZE, fp)) == NULL) {
            printf("Cannot get strings ");
            return -1;
        }
    }
    char *last_string = array[number_of_strings - 1];
    size_t last_string_length = strlen(last_string);
    if(last_string[last_string_length - 1] != '\n'){
        last_string[last_string_length] = '\n';
        last_string[last_string_length + 1] = '\0';
    }
    fclose(fp);
    return 0;
}

int writing_strings(const char *filename, strings_array_t array, int number_of_strings) {
    FILE *mf;
    if((mf = fopen(filename, "wt")) == NULL) {
        printf("File was not opened ");
        return -1;
    }
    for(int i = 0; i < number_of_strings; i++) {
        if((fputs(array[i], mf)) == -1) {
            printf("Cannot write strings ");
            return -1;
        }
    }
    fclose(mf);
    return 0;
}

int main(int argc,char **argv) {
    input_arguments arguments;
    int input_result = getting_the_arguments(argc, argv, &arguments);
    if (input_result != 0)
        return input_result;
    strings_array_t array = NULL;
    int allocation_result = allocate_memory(&array, arguments.number_of_strings);
    if (allocation_result != 0) {
        return allocation_result;
    }
    int reading_result = reading_strings(arguments.input_filename, array, arguments.number_of_strings);
    if (reading_result != 0) {
        return reading_result;
    }
    arguments.sort_func(array, arguments.number_of_strings, arguments.comparer_type);
    int writing_result = writing_strings(arguments.output_filename, array, arguments.number_of_strings);
    if (writing_result != 0) {
        return writing_result;
    }
    clear_memory(&array, arguments.number_of_strings);
    return 0;
}
