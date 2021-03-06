/*
 * Copyright 2000-2012 JetBrains s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define REALLOC_FACTOR 2

struct __array {
  void** data;
  int size;
  int capacity;
};

static bool array_realloc(array* a) {
  if (a->size == a->capacity) {
    int new_cap = a->capacity * REALLOC_FACTOR;
    void* new_ptr = realloc(a->data, sizeof(void*) * new_cap);
    if (new_ptr == NULL) {
      return false;
    }
    a->capacity = new_cap;
    a->data = new_ptr;
  }
  return true;
}

array* array_create(int initial_capacity) {
  array* a = (array*) malloc(sizeof(array));
  if (a == NULL) {
    return NULL;
  }

  a->data = calloc(sizeof(void*), initial_capacity);
  if (a->data == NULL) {
    free(a);
    return NULL;
  }

  a->capacity = initial_capacity;
  a->size = 0;

  return a;
}

inline int array_size(array* a) {
  return (a != NULL ? a->size : 0);
}

void* array_push(array* a, void* element) {
  if (a == NULL || !array_realloc(a)) {
    return NULL;
  }
  a->data[a->size++] = element;
  return element;
}

void* array_pop(array* a) {
  if (a != NULL && a->size > 0) {
    return a->data[--a->size];
  }
  else {
    return NULL;
  }
}

void array_put(array* a, int index, void* element) {
  if (a != NULL && index >=0 && index < a->capacity) {
    a->data[index] = element;
    if (a->size <= index) {
      a->size = index + 1;
    }
  }
}

void* array_get(array* a, int index) {
  if (a != NULL && index >= 0 && index < a->size) {
    return a->data[index];
  }
  else {
    return NULL;
  }
}

void array_delete(array* a) {
  if (a != NULL) {
    free(a->data);
    free(a);
  }
}

void array_delete_vs_data(array* a) {
  if (a != NULL) {
    array_delete_data(a);
    array_delete(a);
  }
}

void array_delete_data(array* a) {
  int i;
  if (a != NULL) {
    for (i=0; i<a->size; i++) {
      if (a->data[i] != NULL) {
        free(a->data[i]);
      }
    }
    a->size = 0;
  }
}

typedef struct node {
    int key;
    void *value;
    struct node *next;
} node;

struct __table {
    node* nodes[16];
};

table* table_create() {
  table* t = malloc(sizeof(table));
  if (t == NULL) {
    return NULL;
  }

  memset(t->nodes, 0, sizeof(struct node*) * 16);

  return t;
}

static inline int wrap(int key, table* t) {
  return (t != NULL ? key % 16 : -1);
}

int table_put(table* t, int key, void* value) {
    int k = wrap(key, t);
    if (k < 0) {
        return 1;
    }
    node *prev = NULL, *n;
    node **ptr = &t->nodes[k];
    if (t->nodes[k] != NULL) {
        for (n = t->nodes[k]; n != NULL; n = n->next) {
            if (n->key == key && value != NULL) {
                /*no replace*/
                return 2;
            } else if (n->key == key) {
                if (prev == NULL) {
                    t->nodes[k] = n->next;
                } else {
                    prev->next = n->next;
                }
                free(n);
                return 0;
            }
            prev = n;
        }
        ptr = &prev->next;
    }
    if (value == NULL) {
        return 0;
    }

    node* new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        return 3;
    }
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    *ptr = new_node;
    return 0;
}

void* table_get(table* t, int key) {
    int k = wrap(key, t);
    if (k < 0) {
        return NULL;
    }
    node *n;
    for (n = t->nodes[k]; n != NULL; n = n->next) {
        if (n->key == key) return n->value;
    }
    return NULL;
}

void table_delete(table* t) {
    int i;
    for (i = 0; i < 16; i++) {
        node* n = t->nodes[i];
        for (;n != NULL; n = n->next) {
            free(n);
        }
    }
    free(t);
}


#define INPUT_BUF_LEN 2048
static char input_buf[INPUT_BUF_LEN];

char* read_line(FILE* stream) {
  char* retval = fgets(input_buf, INPUT_BUF_LEN, stream);
  if (retval == NULL || feof(stream)) {
    return NULL;
  }
  int pos = strlen(input_buf) - 1;
  if (input_buf[pos] == '\n') {
    input_buf[pos] = '\0';
  }
  return input_buf;
}


bool is_parent_path(const char* parent_path, const char* child_path) {
  size_t parent_len = strlen(parent_path);
  return strncmp(parent_path, child_path, parent_len) == 0 &&
         (parent_len == strlen(child_path) || child_path[parent_len] == '/');
}
