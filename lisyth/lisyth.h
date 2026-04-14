#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct str_array {
  char **values;
  size_t size;
  size_t capacity;
} str_array;

typedef struct process {
  str_array *source;
  str_array *links;
  str_array *flags;
  str_array *defs;
  char *compiler;
  char *output;
} process;

str_array* create_str_array();
void set_compiler(process *ptr, char *compiler);
void add_flag(process *ptr, char *flag);
void add_file(process *ptr, char *file);
void add_link(process *ptr, char *link);
void add_define(process *ptr, char *name, char* value);
void set_output(process *ptr, char *output);
void compile(process *ptr);
process *create_process();

typedef struct string {
  char *value;
  size_t len;
} string;

#define string_append(src, value)                                              \
  _Generic((value),                                                            \
      char *: string_append_constant,                                          \
      string *: string_append_dynamic)(src, value)

string *string_creater(char *value);
void string_append_constant(string *src, char *value);
void string_append_dynamic(string *src, string *value);
void str_array_append(str_array* arr, char *value);

#ifdef LISYTH_IMPLEMENTATION
void str_array_append(str_array* arr, char *value) {
   if (arr->size >= arr->capacity) {
    arr->capacity *= 2;
    arr->values =
        realloc(arr->values, sizeof(char *) * arr->capacity);
  }

  arr->values[arr->size++] = value; 
}

str_array *create_str_array() {
  str_array *arr = malloc(sizeof(str_array));
  arr->size = 0;
  arr->capacity = 16;
  arr->values = malloc(sizeof(char *) * arr->capacity);
  return arr;
}

string *string_create(char *value) {
  string *str = malloc(sizeof(string));
  size_t len = strlen(value);
  str->value = malloc(len + 1);
  memcpy(str->value, value, len);
  str->value[len] = '\0';
  str->len = strlen(value);
  return str;
}

void string_append_constant(string *dest, char *value) {
  size_t value_len = strlen(value);
  dest->value = realloc(dest->value, dest->len + value_len + 1);;

  memcpy(dest->value + dest->len, value, value_len);
  dest->len += value_len;
  dest->value[dest->len] = '\0';
}

void string_append_dynamic(string *dest, string *value) {
  size_t value_len = value->len;
  dest->value = realloc(dest->value, dest->len + value_len + 1);

  memcpy(dest->value + dest->len, value->value, value_len);
  dest->len += value_len;
  dest->value[dest->len] = '\0';
}

process *create_process() {
  process *ptr = malloc(sizeof(process));
  ptr->output = "a.out";
  ptr->compiler = "gcc";
  ptr->source = create_str_array();
  ptr->flags = create_str_array();
  ptr->links = create_str_array();
  ptr->defs = create_str_array();
  return ptr;
}

void set_output(process *ptr, char *output) { ptr->output = output; }

void set_compiler(process *ptr, char *compiler) { ptr->compiler = compiler; }

void add_file(process *ptr, char *file) {
  str_array_append(ptr->source, file);
}

void add_link(process *ptr, char *link) {
  str_array_append(ptr->links, link);
}

void add_flag(process *ptr, char *flag) {
  str_array_append(ptr->flags, flag);
}

void add_define(process *ptr, char* name, char* value) {
  str_array_append(ptr->defs, name);
  str_array_append(ptr->defs, value);
}

void compile(process *ptr) {
  string *command = string_create(ptr->compiler);
  string_append(command, " -o ");
  string_append(command, ptr->output);
  string_append(command, " ");

  for (int index = 0; index < ptr->source->size; ++index) {
    string_append(command, ptr->source->values[index]);
    string_append(command, " ");
  }

  for (int index = 0; index < ptr->links->size; ++index) {
    string_append(command, "-l");
    string_append(command, ptr->links->values[index]);
    string_append(command, " ");
  }

  for (int index = 0; index < ptr->flags->size; ++index) {
    string_append(command, "-");
    string_append(command, ptr->flags->values[index]);
    string_append(command, " ");
  }

  for (int index = 0; index < ptr->defs->size; index+=2) {
    string_append(command, "-D");
    string_append(command, ptr->defs->values[index]);
    string_append(command, "=");
    string_append(command, ptr->defs->values[index + 1]);
    string_append(command, " ");
  }
  printf("%s\n", command->value);
  system(command->value);
}
#endif
