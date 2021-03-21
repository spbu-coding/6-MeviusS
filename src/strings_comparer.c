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

int getting_the_arguments(int argc, char **argv, input_arguments *arguments) {
    if (argc != NUMBER_OF_ARGS) {
        printf("Incorrect number of parameters ");
        return -1;
    }
    char arg_buf[MAX_INPUT_STRING_SIZE];
    for (int i = 0; i < MAX_INPUT_STRING_SIZE; i++) {
        arg_buf[i] = '1';
    }
    strncpy(arg_buf, argv[1], strlen(arg_buf));
    size_t i = 0;
    size_t length_of_buf = strlen(arg_buf);
    for (; i < length_of_buf ; i++) {
        if (arg_buf[i] < '0' || arg_buf[i] > '9') {
            printf("First parameter must be only a number ");
            break;
        }
    }
    if (length_of_buf > i) {
        printf("First parameter must be only a number ");
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
        printf("Incorrect sorting name");
        return -1;
    }
    if (strcmp(argv[5], "asc") == 0) {
        arguments->comparer_type = asc_comparer_type;
    } else if (strcmp(argv[5], "des") == 0) {
        arguments->comparer_type = des_comparer_type;
    } else {
        printf("Incorrect comparer type");
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
        printf("Cannot allocate memory");
        free(*array);
        return -1;
    }
    for (int i = 0; i < number_of_strings; i++) {
        if (((*array)[i] = malloc(sizeof(char) * MAX_INPUT_STRING_SIZE)) == NULL) {
            printf("Cannot allocate memory");
            clear_memory(array, i);
            return -1;
        }
    }
    return 0;
}

int reading_strings(const char *filename, strings_array_t array, int number_of_strings) {
    FILE *fp;
    if((fp = fopen(filename, "r")) == NULL) {
        printf("File was not opened");
        return -1;
    }

    for(int i = 0; i < number_of_strings; i++) {
        if((fgets(array[i], MAX_INPUT_STRING_SIZE, fp)) == NULL) {
            printf("Cannot get strings");
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
    if((mf = fopen(filename, "w")) == NULL) {
        printf("File was not opened");
        return -1;
    }
    for(int i = 0; i < number_of_strings; i++) {
        if((fputs(array[i], mf)) == EOF) {
            printf("Cannot write strings");
            return -1;
        }
    }
    fclose(mf);
    return 0;
}

int main(int argc,char **argv) {
    input_arguments arguments;
    int result = getting_the_arguments(argc, argv, &arguments);
    if (result != 0)
        return result;
    strings_array_t array = malloc(sizeof (char *) * arguments.number_of_strings);
    allocate_memory(&array, arguments.number_of_strings);
    reading_strings(arguments.input_filename, array, arguments.number_of_strings);
    arguments.sort_func(array, arguments.number_of_strings, arguments.comparer_type);
    writing_strings(arguments.output_filename, array, arguments.number_of_strings);
    clear_memory(&array, arguments.number_of_strings);
    return 0;
}