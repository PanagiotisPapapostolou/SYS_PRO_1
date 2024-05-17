#include "string_array.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10  // Initial capacity of the array

// Function to initialize a dynamic array of strings
StringArray *string_array_init() {
    StringArray *arr = (StringArray *)malloc(sizeof(StringArray));
    if (arr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    arr->data = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
    if (arr->data == NULL) {
        perror("malloc");
        free(arr);
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY;
    return arr;
}

// Function to add a string to the dynamic array and tokenize it based on the given delimiter
void string_array_add(StringArray *arr, const char *str, const char *delimiter) {
    char *token = strtok(str, delimiter);
    while (token != NULL) {
        // Resize the array if necessary
        if (arr->size >= arr->capacity) {
            arr->capacity *= 2;
            arr->data = (char **)realloc(arr->data, arr->capacity * sizeof(char *));
            if (arr->data == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        // Copy the token and add it to the array
        arr->data[arr->size] = strdup(token);
        if (arr->data[arr->size] == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        arr->size++;

        // Get the next token
        token = strtok(NULL, delimiter);
    }
}

// Function to free memory allocated by a dynamic array
void string_array_free(StringArray *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        free(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}