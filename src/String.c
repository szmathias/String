#include "String.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to allocate zeroed memory
static void *mem_calloc(size_t num_elements, size_t element_size)
{
    void *result = calloc(num_elements, element_size);
    if (!result)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

// Helper function to ensure enough capacity for data
static void str_ensure_capacity(String *str, size_t required_capacity) 
{
    if (required_capacity >= str->capacity) 
    {
        size_t new_capacity = str->capacity + (str->capacity >> 1); // 1.5 times the current capacity
        if (new_capacity < required_capacity)  
        {
            new_capacity = required_capacity;
        }

        char *temp_data = (char *)mem_calloc(new_capacity, sizeof(char));
        if (str->capacity != STR_MIN_INIT_CAP) 
        {
            memcpy(temp_data, str->data, str->size);
            free(str->data);
        } 
        else 
        {
            memcpy(temp_data, str->small_data, str->size);
        }

        str->data = temp_data;
        str->capacity = new_capacity;
    }
}

String str_create_empty(size_t initial_capacity)
{
    String result;

    size_t capacity = (initial_capacity >= STR_MIN_INIT_CAP) ? initial_capacity : STR_MIN_INIT_CAP;
    
    result.capacity = capacity;
    result.size = 0;

    if (capacity > STR_MIN_INIT_CAP) {
        result.data = mem_calloc(capacity, sizeof(char));
    } 
    else 
    {
        // Use the small_data buffer for small initial capacities
        memset(result.small_data, '\0', STR_MIN_INIT_CAP);
        result.data = NULL;
    }

    return result;
}

String str_create_from_cstring(const char *cstr)
{
    size_t length = strlen(cstr);
    String result = str_create_empty(length);
    char *data_to_use = str_data(&result);
    memcpy(data_to_use, cstr, length);
    result.size = length;

    return result;
}

String str_create_from_string(String *str)
{
    char *data_to_use = str_data(str);
    return str_create_from_cstring(data_to_use);
}

void str_free(String *str)
{
    if (str->capacity != STR_MIN_INIT_CAP)
    {
        free(str->data);
    }
    str->size = 0;
    str->capacity = 0;
    str->data = NULL;
}

void str_assign_char(String *str, char value)
{
    str_clear(str);
    char *data_to_use = str_data(str);
    data_to_use[0] = value;
    str->size = 1;
}

void str_assign_cstring(String *str, const char *cstr)
{
    str_clear(str);
    size_t length = strlen(cstr);
    str_ensure_capacity(str, length);
    char *data_to_use = str_data(str);
    memcpy(data_to_use, cstr, length + 1);
    str->size = length;
}

void str_assign_string(String *str, String *from)
{
    char *data_to_use = str_data(from); 
    str_assign_cstring(str, data_to_use);
}

void str_push_back_char(String *str, char value)
{
    str_ensure_capacity(str, str->size + 1); // Ensure enough capacity for the new character

    if (str->capacity == STR_MIN_INIT_CAP)
    {
        str->small_data[str->size] = value;
    }
    else
    {
        str->data[str->size] = value;
    }

    ++str->size;
}

void str_push_back_cstring(String *str, const char *cstr)
{
    size_t length = strlen(cstr);
    str_ensure_capacity(str, str->size + length); // Ensure enough capacity for the entire C-string

    char *data_to_use = str_data(str);
    memcpy(data_to_use + str->size, cstr, length); // +1 to include the null-terminator

    str->size += length;
}

void str_push_back_string(String *str, String *from)
{
    char *data_to_use = str_data(from);
    str_push_back_cstring(str, data_to_use);
}

void str_insert_char(String *str, size_t pos, char value)
{
    if (pos > str->size)
    {
        return;
    }

    if (pos == str->size)
    {
        str_push_back_char(str, value);
    }
    else
    {
        char *data_to_use = str_data(str);

        if (str->size + 1 > str->capacity - 1)
        {
            char *temp = mem_calloc(str->capacity * 2, sizeof(char));

            size_t i = 0;
            for (; i < pos; i++)
            {
                temp[i] = data_to_use[i];
            }
            temp[i] = value;
            for (; i < str->size; i++)
            {
                temp[i + 1] = data_to_use[i];
            }

            if (str->capacity != STR_MIN_INIT_CAP)
            {
                free(str->data);
            }
            str->data = temp;
            str->size++;
            str->capacity *= 2;
        }
        else
        {
            for (size_t i = str->size; i > pos; i--)
            {
                data_to_use[i] = data_to_use[i - 1];
            }
            data_to_use[pos] = value;
            str->size++;
        }
    }
}

void str_insert_cstring(String *str, size_t pos, const char *cstr)
{
    if (pos > str->size)
    {
        return;
    }

    if (pos == str->size)
    {
        str_push_back_cstring(str, cstr);
    }
    else
    {
        for (size_t i = 0; i < strlen(cstr); i++)
        {
            str_insert_char(str, pos + i, cstr[i]);
        }
    }
}

void str_insert_string(String *str, size_t pos, String *from)
{
    char *data_to_use = str_data(from);

    str_insert_cstring(str, pos, data_to_use);
}

void str_pop_back(String *str)
{
    if (str->size > 0)
    {
        char *data_to_use = str_data(str);
        str->size--;
        data_to_use[str->size] = '\0';
    }
}

void str_erase(String *str, size_t pos)
{
    if (str->size == 0 || pos >= str->size)
    {
        return;
    }

    if (pos == str->size - 1)
    {
        str_pop_back(str);
    }
    else
    {
        char *data_to_use = str_data(str);

        for (size_t i = pos; i < str->size - 1; i++)
        {
            data_to_use[i] = data_to_use[i + 1];
        }
        str->size--;
        data_to_use[str->size] = '\0';
    }
}

bool str_empty(String *str)
{
    return (str->size == 0);
}

void str_clear(String *str)
{
    char *data_to_use = str_data(str);

    memset(data_to_use, 0, str->size);
    str->size = 0;
}

char *str_data(String *str)
{
    char *data_to_use = str->data;
    if (str->capacity == STR_MIN_INIT_CAP)
    {
        data_to_use = str->small_data;
    }
    return data_to_use;
}

size_t str_capacity(String *str)
{
    return str->capacity;
}

size_t str_size(String *str)
{
    return str->size;
}

size_t str_find_first_of(String *str, const char *value)
{
    char *data_to_use = str_data(str);

    char *result = strpbrk(data_to_use, value);
    if (result != NULL)
    {
        return (size_t)(result - data_to_use);
    }
    else
    {
        return STR_NPOS;
    }
}

size_t str_find_cstring(String *str, const char *find)
{
    size_t length = strlen(find);
    if (length == 0)
    {
        return STR_NPOS;
    }

    char *data_to_use = str_data(str);

    size_t pos = STR_NPOS;
    for (size_t i = 0; i < str->size; i++)
    {
        if (data_to_use[i] == find[0])
        {
            if (strncmp(data_to_use + i, find, length) == 0)
            {
                pos = i;
                break;
            }
        }
    }
    return pos;
}

size_t str_find_string(String *str, String *find)
{
    char *data_to_use = str_data(find);
    return str_find_cstring(str, data_to_use);
}

void str_trim_front(String *str)
{
    char *data = str_data(str);
    size_t i = 0;
    
    while (i < str->size && isspace((unsigned char)data[i]))
    {
        i++;
    }

    if (i == 0)
    {
        return;
    }

    size_t count = str->size - i;
    if(count == 0)
    {
        str_clear(str);
    }
    else 
    {
        memmove(data, data + i, count);
        for(size_t j = 0; j < i; ++j)
        {
            str_pop_back(str);
        }
    }
}

void str_trim_back(String *str)
{
    if(str->size == 0)
    {
        return;
    }
    
    char *data = str_data(str);
    while(isspace(data[str->size - 1]))
    {
        str_pop_back(str);
    }
}

void str_remove_extra_ws(String *str)
{
    char *data_to_use = str_data(str);

    str_trim_back(str);
    str_trim_front(str);
    for (size_t i = 0; i < str->size; ++i)
    {
        if (isspace((unsigned char)data_to_use[i]) != 0 && isspace((unsigned char)data_to_use[i + 1]) != 0)
        {
            if(data_to_use[i] != ' ')
            {
                data_to_use[i] = ' ';
            }
            str_erase(str, i);
            --i;
        }
    }
}

void str_to_lower(String *str)
{
    char *data_to_use = str_data(str);
    for (size_t i = 0; i < str->size; i++)
    {
        data_to_use[i] = tolower((unsigned char)data_to_use[i]);
    }
}

void str_to_upper(String *str)
{
    char *data_to_use = str_data(str);
    for (size_t i = 0; i < str->size; i++)
    {
        data_to_use[i] = toupper((unsigned char)data_to_use[i]);
    }
}

String str_substr_create_cstring(const char *cstr, size_t pos, size_t count)
{
    size_t size = strlen(cstr);

    if (count + pos >= size)
    {
        count = size - pos;
    }

    if (size - count > size)
    {
        return str_create_empty(0);
    }

    if (pos >= size)
    {
        return str_create_empty(0);
    }

    String result = str_create_empty(count);
    for (size_t i = 0; i < count; i++)
    {
        str_push_back_char(&result, cstr[i + pos]);
    }

    return result;
}

String str_substr_create_string(String *str, size_t pos, size_t count)
{
    char *data_to_use = str_data(str);
    return str_substr_create_cstring(data_to_use, pos, count);
}

char *str_substr_cstring(const char *cstr, size_t pos, size_t count, char *buffer)
{
    size_t size = strlen(cstr);
    if (count + pos >= size)
    {
        count = size - pos;
    }

    if (size - count > size)
    {
        return buffer;
    }

    if (pos >= size)
    {
        return buffer;
    }

    for (size_t i = 0; i < count; i++)
    {
        buffer[i] = cstr[i + pos];
    }
    buffer[count] = '\0';
    return buffer;
}

char *str_substr_string(String *str, size_t pos, size_t count, char *buffer)
{
    char *data_to_use = str_data(str);
    return str_substr_cstring(data_to_use, pos, count, buffer);
}

int str_compare_cstring(String *lhs, const char *rhs)
{
    char *data_to_use = str_data(lhs);

    size_t rhs_size = strlen(rhs);
    size_t min = (lhs->size < strlen(rhs)) ? lhs->size : rhs_size;

    int result = strncmp(data_to_use, rhs, min);
    if (result != 0)
    {
        return result;
    }
    if (lhs->size < rhs_size)
    {
        result = -1;
    }
    else if (lhs->size > rhs_size)
    {
        result = 1;
    }
    return result;
}

int str_compare_string(String *lhs, String *rhs)
{
    char *rhs_data_to_use = str_data(rhs);
    return str_compare_cstring(lhs, rhs_data_to_use);
}

int str_getline_ch(FILE *stream, String *line, int delim)
{
    str_clear(line);
    int ch = 0;
    int status = 0;

    if(delim == '\0')
    {
        delim = '\n';
    }

    while ((ch = fgetc(stream)) != EOF && ch != delim)
    {
        str_push_back(line, ch);
    }

    if (feof(stream))
    {
        status = EOF;
    }
    return status;
}

int str_getline_cstr(FILE *stream, String *line, const char *delim)
{
    str_clear(line);
    const char *sentry = "\n";
    if (delim != NULL && strlen(delim) != 0)
    {
        sentry = delim;
    }

    int ch = 0;
    int status = 0;

    while ((ch = fgetc(stream)) && ch != EOF)
    {
        if (strchr(sentry, ch) != NULL)
        {
            break;
        }
        str_push_back(line, ch);
    }

    if (ch == EOF)
    {
        status = EOF;
    }
    return status;
}

int str_getline_str(FILE *stream, String *line, String *delim)
{
    char *delim_data = str_data(delim);
    return str_getline_cstr(stream, line, delim_data);
}
