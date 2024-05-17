#include <stdio.h>
// Structure to represent a dynamic array of strings
typedef struct {
    char **data;    // Pointer to the array of strings
    size_t size;    // Number of elements currently stored in the array
    size_t capacity;    // Maximum capacity of the array
} StringArray;

// Function to initialize a dynamic array of strings
StringArray *string_array_init();

// Function to add a string to the dynamic array and tokenize it based on the given delimiter
void string_array_add(StringArray *arr, const char *str, const char *delimiter);

// Function to free memory allocated by a dynamic array
void string_array_free(StringArray *arr);