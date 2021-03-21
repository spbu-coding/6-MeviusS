#include <stdio.h>
#include <stdlib.h>

#pragma once

#include <string.h>

#define  MAX_INPUT_STRING_SIZE 1000

#define ASCII_LENGTH 128

typedef int (*comparator_func_t)(const char*, const char*);
typedef char** strings_array_t;
typedef size_t array_size_t;

void radix(strings_array_t, array_size_t, comparator_func_t);

array_size_t search_the_max_length(strings_array_t strings, size_t strings_count) {
    size_t max_length = strlen(strings[0]);
    for (size_t i = 0; i < strings_count; i++) {
        size_t temp_length = strlen(strings[i]);
        if (temp_length > max_length) {
            max_length = temp_length;
        }
    }
    return max_length;
}

void count_sort(strings_array_t array, size_t size, size_t k, comparator_func_t cmp) {
    strings_array_t temp_array = malloc(sizeof(char *) * size);
    int *ascii_array = malloc(sizeof(int) * ASCII_LENGTH);
    for (int i = 0; i < ASCII_LENGTH; ++i) {
        ascii_array[i] = 0;
    }
    for (size_t i = 0; i < size; ++i) {
        int temp = k < strlen(array[i]) ? (int)(unsigned char)array[i][k] + 1 : 0;
        ascii_array[temp]++;
    }
    if (cmp("a", "temp_array") < 0) {
        for (int i = 1; i < ASCII_LENGTH; ++i) {
            ascii_array[i] += ascii_array[i - 1];
        }
    } else {
        for (int i = ASCII_LENGTH - 2; i >=0; --i) {
            ascii_array[i] += ascii_array[i + 1];
        }
    }
    for (int i = (int)size - 1; i >= 0; --i) {
        temp_array[ascii_array[k < strlen(array[i]) ? (int)(unsigned char)array[i][k] + 1 : 0] - 1] = array[i];
        ascii_array[k < strlen(array[i]) ? (int)(unsigned char)array[i][k] + 1 : 0]--;
    }
    for (size_t i = 0; i < size; ++i) {
        array[i] = temp_array[i];
    }
    free(temp_array);
    free(ascii_array);
}

void swap(char **str_1, char **str_2) {
    char *temp = *str_1;
    *str_1 = *str_2;
    *str_2 = temp;
}

void bobble(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            if(cmp(array[j], array[i]) < 0) {
                swap(&array[i], &array[j]);
            }
        }
    }
}

void insertion(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    for(array_size_t i = 0; i < size; i++) {
        char *temp = array[i];
        for(int j = (int)i - 1; j >= 0; j--) {
            if(cmp(array[j], temp) < 0) {
                break;
            }
            array[j + 1] = array[j];
            array[j] = temp;
        }
    }
}

void merge(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    array_size_t step = 1;
    strings_array_t temp = malloc(sizeof(char *) * size);
    while(step < size) {
        array_size_t index = 0;
        array_size_t left = 0;
        array_size_t mid = left + step;
        array_size_t right = left + step * 2;
        do {
            mid = mid < size ? mid : size;
            right = right < size ? right : size;
            array_size_t i = left, j = mid;
            for (; i < mid && j < right;) {
                if (cmp(array[i], array[j]) < 0) {
                    temp[index++] = array[i++];
                } else {
                    temp[index++] = array[j++];
                }
            }
            while (i < mid) {
                temp[index++] = array[i++];
            }
            while (j < right) {
                temp[index++] = array[j++];
            }
            left += step * 2;
            mid += step * 2;
            right += step * 2;
        } while(left < size);
        for(array_size_t i1 = 0; i1 < size; i1++) {
            array[i1] = temp[i1];
        }
        step *= 2;
    }
    free(temp);
}

void quick_split(strings_array_t array, unsigned int beg, unsigned int end, comparator_func_t cmp) {
    while (beg < end) {
        if ((array[beg] <= array[(beg + end - 1) / 2] && array[(beg + end - 1) / 2] <= array[end - 1]) || (array[end - 1] <= array[(beg + end - 1) / 2] && array[(beg + end - 1) / 2] <= array[beg])) {
            swap(&array[beg], &array[(beg + end - 1) / 2]);
        } else if ((array[beg] <= array[end - 1] && array[end - 1] <= array[(beg + end - 1) / 2]) || (array[(beg + end - 1) / 2] <= array[end - 1] && array[end - 1] <= array[beg])) {
            swap(&array[beg], &array[end - 1]);
        }
        unsigned int left = beg, mid = beg + 1, right = end;
        for (unsigned int i = 0; i < end - beg - 1; i++) {
            const int cmp_result = cmp(array[mid], array[beg]);
            if (cmp_result < 0) {
                left++;
                swap(&array[mid], &array[left]);
                mid++;
            } else if (cmp_result > 0) {
                right--;
                swap(&array[mid], &array[right]);
            } else {
                mid++;
            }
        }
        swap(&array[beg], &array[left]);
        quick_split(array, beg, left, cmp);
        beg = right;
    }
}

void quick(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    quick_split(array, 0, size, cmp);
}

void radix(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    size_t max = search_the_max_length(array, size);
    for (size_t digit = max; digit > 0; digit--) {
        count_sort(array, size, digit - 1, cmp);
    }
}
