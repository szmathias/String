#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Minimum initial string capacity
#define STR_MIN_INIT_CAP 16

// Represents position not found
#define STR_NPOS SIZE_MAX

typedef struct String
{
    size_t capacity;
    size_t size;
    
    union
    {
      char* data;
      char small_data[STR_MIN_INIT_CAP];
    };
} String;

// Convenience macros to hopefully save some typing
// The generic implementation of these macros are towards the bottom of the
// header file so they don't clog up the rest of the API
#define str_create(value)             str_create_impl(value)
#define str_assign(str, value)        str_assign_impl(str, value)
#define str_push_back(str, value)     str_push_back_impl(str, value)
#define str_insert(str, pos, value)   str_insert_impl(str, pos, value)
#define str_find_str(str, find)       str_find_str_impl(str, find)
#define str_substr(str, pos, count)   str_substr_impl(str, pos, count)
#define str_compare(lhs, rhs)         str_compare_impl(lhs, rhs)

// Different creation functions
String str_create_empty(size_t initial_capacity);
String str_create_from_cstring(const char* cstr);
String str_create_from_string(String* str);

// String free method
void str_free(String* str);

// Clears the content of your string and assigns the value given
void str_assign_char(String* str, char value);
void str_assign_cstring(String* str, const char* cstr);
void str_assign_string(String* str, String* from);

// Appends to the end of your string
void str_push_back_char(String* str, char value);
void str_push_back_cstring(String* str, const char* cstr);
void str_push_back_string(String* str, String* from);

// Inserts the value into any valid position within the string
void str_insert_char(String* str, size_t pos, char value);
void str_insert_cstring(String* str, size_t pos, const char* cstr);
void str_insert_string(String* str, size_t pos, String* from);

// Pop back gets rid of the last char in the string
// Erase gets rid of a character at the position given then shifts the strings content
void str_pop_back(String* str);
void str_erase(String* str, size_t pos);

// Empty returns whether the string is empty or not
// Clear resets the strings content and size to zero
bool str_empty(String* str);
void str_clear(String* str);

// Helper functions for internal data
char* str_data(String* str);
size_t str_capacity(String* str);
size_t str_size(String* str);

// Finds the first character that matches any character in value and
// returns it's position or STR_NPOS
size_t str_find_first_of(String* str, const char* value);

// Searches for the string given and returns the position of the beginning
// of that string or STR_NPOS //
size_t str_find_cstring(String* str, const char* find);
size_t str_find_string(String* str, String* find);

// Removes whitespace from the front and back of the string then gets rid
// of duplicate whitespace in the middle
void str_remove_extra_ws(String* str);

// Sets the entire string to lowercase or uppercase
void str_to_lower(String* str);
void str_to_upper(String* str);

// Returns a new string that is a sub string of the original or an empty
// string if pos is invalid
String str_sub_cstring(const char* cstr, size_t pos, size_t count);
String str_sub_string(String* str, size_t pos, size_t count);

int str_compare_cstring(String* lhs, const char* rhs);
int str_compare_string(String* lhs, String* rhs);

// The generic macro implementations for convenience
#define str_create_impl(value) _Generic(\
                                value,\
                                char*: str_create_from_cstring,\
                                const char*: str_create_from_cstring,\
                                String*: str_create_from_string,\
                                default: str_create_empty\
                              )(value)

#define str_assign_impl(str, value) _Generic(\
                                value,\
                                char*: str_assign_cstring,\
                                const char*: str_assign_cstring,\
                                String*: str_assign_string,\
                                default: str_assign_char\
                              )(str, value)

#define str_push_back_impl(str, value) _Generic(\
                                value,\
                                char*: str_push_back_cstring,\
                                const char*: str_push_back_cstring,\
                                String*: str_push_back_string,\
                                default: str_push_back_char\
                              )(str, value)

#define str_insert_impl(str, pos, value) _Generic(\
                                value,\
                                char*: str_insert_cstring,\
                                const char*: str_insert_cstring,\
                                String*: str_insert_string,\
                                default: str_insert_char\
                              )(str, pos, value)

#define str_find_str_impl(str, find) _Generic(\
                                find,\
                                char*: str_find_cstring,\
                                const char*: str_find_cstring,\
                                default: str_find_string\
                              )(str, find)

#define str_substr_impl(str, pos, count) _Generic(\
                                str,\
                                char*: str_sub_cstring,\
                                const char*: str_sub_cstring,\
                                default: str_sub_string\
                              )(str, pos, count)

#define str_compare_impl(lhs, rhs) _Generic(\
                                rhs,\
                                char*: str_compare_cstring,\
                                const char*: str_compare_cstring,\
                                default: str_compare_string\
                              )(lhs, rhs)
