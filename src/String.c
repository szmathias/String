#include "String.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void* mem_calloc(size_t num_elements, size_t element_size)
{
    void* result = calloc(num_elements, element_size);
    #ifndef NDEBUG
    if(!result)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    #endif
    return result;
}

String str_create_empty(size_t initial_capacity)
{
    String result = { 0 };
    if(initial_capacity < STR_MIN_INIT_CAP)
    {
        result.capacity = STR_MIN_INIT_CAP;
        result.size = 0;
        for(size_t i = 0; i < STR_MIN_INIT_CAP; i++)
        {
            result.small_data[i] = '\0';
        }
    }
    else
    {
        size_t cap = STR_MIN_INIT_CAP * 2;
        while(cap <= initial_capacity)
        {
            cap *= 2;
        }
        result.capacity = cap;
        result.size = 0;
        result.data = mem_calloc(cap, sizeof(char));
    }

    return result;
}

String str_create_from_cstring(const char* cstr)
{
    String result = str_create_empty(strlen(cstr));
    for(size_t i = 0; i < strlen(cstr); i++)
    {
        str_push_back_char(&result, cstr[i]);
    }

    return result;
}

String str_create_from_string(String* str)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    return str_create_from_cstring(data_to_use);
}

void str_free(String* str)
{
    if(str->capacity != STR_MIN_INIT_CAP)
    {
        free(str->data);
    }
    str->size = 0;
    str->capacity = 0;
    str->data = NULL;
}

void str_assign_char(String* str, char value)
{
    str_clear(str);
    str_push_back_char(str, value);
}

void str_assign_cstring(String* str, const char* cstr)
{
    str_clear(str);
    for(size_t i = 0; i < strlen(cstr); i++)
    {
        str_push_back_char(str, cstr[i]);
    }
}

void str_assign_string(String* str, String* from)
{
    char* data_to_use = from->data;
    if(from->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = from->small_data;
    }

    str_assign_cstring(str, data_to_use);
}

void str_push_back_char(String* str, char value)
{
    if(str->size < str->capacity - 1)
    {
        if(str->capacity == STR_MIN_INIT_CAP)
        {
            str->small_data[str->size] = value;
        }
        else
        {
            str->data[str->size] = value;
        }
        str->size++;
    }
    else
    {
        str->capacity *= 2;
        char* temp = mem_calloc(str->capacity, sizeof(char));
        temp = memcpy(temp, str->small_data, str->size);
        str->data = temp;
        str->data[str->size] = value;
        str->size++;
    }
}

void str_push_back_cstring(String* str, const char* cstr)
{
    for(size_t i = 0; i < strlen(cstr); i++)
    { 
        str_push_back_char(str, cstr[i]);
    }
}

void str_push_back_string(String* str, String* from)
{
    char* data_to_use = from->data;
    if(from->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = from->small_data;
    }
    
    str_push_back_cstring(str, data_to_use);
}

void str_insert_char(String* str, size_t pos, char value)
{
    if(pos > str->size)
    {
        return;
    }

    if(pos == str->size)
    {
        str_push_back_char(str, value);
    }
    else
    {
        char* data_to_use = str->data;
        if(str->capacity == STR_MIN_INIT_CAP)
        {
            data_to_use = str->small_data;
        }

        if(str->size + 1 > str->capacity - 1)
        {
            char* temp = mem_calloc(str->capacity * 2, sizeof(char));
            
            size_t i = 0;
            for (; i < pos; i++)
            {
                temp[i] = data_to_use[i];
            }
            temp[i] = value;
            for(; i < str->size; i++)
            {
                temp[i + 1] = data_to_use[i];
            }

            if(str->capacity != STR_MIN_INIT_CAP)
            {
                free(str->data);
            }
            str->data = temp;
            str->size++;
            str->capacity *= 2;
        }
        else
        {
            for(size_t i = str->size; i > pos; i--)
            {
                data_to_use[i] = data_to_use[i - 1];
            }
            data_to_use[pos] = value;
            str->size++;
        }
    }
}

void str_insert_cstring(String* str, size_t pos, const char* cstr)
{
    if(pos > str->size)
    {
        return;
    }

    if(pos == str->size)
    {
        str_push_back_cstring(str, cstr);
    }
    else
    {
        for(size_t i = 0; i < strlen(cstr); i++)
        {
            str_insert_char(str, pos + i, cstr[i]);
        }
    }
}

void str_insert_string(String* str, size_t pos, String* from)
{
    char* data_to_use = from->data;
    if(from->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = from->small_data;
    }

    str_insert_cstring(str, pos, data_to_use);
}

void str_pop_back(String* str)
{
    if(str->size > 0)
    {
        char* data_to_use = str->data;
        if(str->capacity == STR_MIN_INIT_CAP)
        {
            data_to_use = str->small_data;
        }

        str->size--;
        data_to_use[str->size] = '\0';
    }
}

void str_erase(String* str, size_t pos)
{
    if(str->size == 0 || pos >= str->size)
    {
        return;
    }

    if(pos == str->size - 1)
    {
        str_pop_back(str);
    }
    else
    {
        char* data_to_use = str->data;
        if(str->capacity == STR_MIN_INIT_CAP)
        {
            data_to_use = str->small_data;
        }

        for(size_t i = pos; i < str->size - 1; i++)
        {
            data_to_use[i] = data_to_use[i + 1];
        }
        str->size--;
        data_to_use[str->size] = '\0';
    }
}

bool str_empty(String* str)
{
    return (str->size == 0);
}

void str_clear(String* str)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    memset(data_to_use, 0, str->size);
    str->size = 0;
}

char* str_data(String* str)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    return data_to_use;
}

size_t str_capacity(String* str)
{
    return str->capacity;
}

size_t str_size(String* str)
{
    return str->size;
}

size_t str_find_first_of(String* str, const char* value)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    size_t pos = STR_NPOS;
    size_t value_len = strlen(value);
    for(size_t i = 0; i < str->size && value_len > 0; i++)
    {
        for(size_t j = 0; j < value_len; j++)
        {
            if(data_to_use[i] == value[j])
            {
                pos = i;
                break;
            }
        }
        if(pos != STR_NPOS)
        {
            break;
        }
    }

    return pos;
}

size_t str_find_cstring(String* str, const char* find)
{
    size_t length = strlen(find);
    if(length == 0)
    {
        return STR_NPOS;
    }

    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    size_t pos = STR_NPOS;
    for(size_t i = 0; i < str->size; i++)
    {
        if(data_to_use[i] == find[0])
        {
            if(strncmp(data_to_use + i, find, length) == 0)
            {
                pos = i;
                break;
            }
        }
    }
    return pos;
}

size_t str_find_string(String* str, String* find)
{
    char* data_to_use = find->data;
    if(find->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = find->small_data;
    }

    return str_find_cstring(str, data_to_use);
}

void str_remove_extra_ws(String* str)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    while(!str_empty(str) && isspace((unsigned char)data_to_use[0]) != 0)
    {
        str_erase(str, 0);
    }

    while(!str_empty(str) && isspace((unsigned char)data_to_use[str->size - 1]) != 0)
    {
        str_pop_back(str);
    }

    for(size_t j = 0; j < str->size; j++)
    {
        if(isspace((unsigned char)data_to_use[j]) != 0 && isspace((unsigned char)data_to_use[j + 1]) != 0)
        {
            str_erase(str, j);
            j--;
        }
    }
}

void str_to_lower(String* str)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }
    
    for(size_t i = 0; i < str->size; i++)
    {
        data_to_use[i] = tolower(data_to_use[i]);
    }
}

void str_to_upper(String* str)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    for(size_t i = 0; i < str->size; i++)
    {
        data_to_use[i] = toupper(data_to_use[i]);
    }
}

String str_sub_cstring(const char* cstr, size_t pos, size_t count)
{
    size_t size = strlen(cstr);

    if(count + pos >= size)
    {
        count = size - pos;
    }
    
    if(size - count > size)
    {
        return str_create_empty(0);
    }
    
    if(pos >= size)
    {
        return str_create_empty(0);
    }
    
    String result = str_create_empty(count);
    for(size_t i = 0; i < count; i++)
    {
        str_push_back_char(&result, cstr[i + pos]);
    }

    return result;
}

String str_sub_string(String* str, size_t pos, size_t count)
{
    char* data_to_use = str->data;
    if(str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }

    return str_sub_cstring(data_to_use, pos, count);
}

int str_compare_cstring(String* lhs, const char* rhs)
{
    char* data_to_use = lhs->data;
    if(lhs->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = lhs->small_data;
    }

    size_t size = (lhs->size < strlen(rhs)) ? lhs->size : strlen(rhs);

    return strncmp(data_to_use, rhs, size);
}

int str_compare_string(String* lhs, String* rhs)
{
    char* lhs_data_to_use = lhs->data;
    if(lhs->capacity == STR_MIN_INIT_CAP)
    {
        lhs_data_to_use = lhs->small_data;
    }

    char* rhs_data_to_use = rhs->data;
    if(rhs->capacity == STR_MIN_INIT_CAP)
    {
        rhs_data_to_use = rhs->small_data;
    }

    size_t size = (lhs->size < rhs->size) ? lhs->size : rhs->size;

    return strncmp(lhs_data_to_use, rhs_data_to_use, size);
}