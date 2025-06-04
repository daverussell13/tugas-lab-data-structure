#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <stdbool.h>

typedef int (*HeapCompareFunc)(const void*, const void*);

typedef struct
{
  void** array;
  size_t capacity;
  size_t size;
  size_t element_size;
  HeapCompareFunc compare;
} Heap;

Heap* heap_create(size_t initial_capacity, size_t element_size, HeapCompareFunc compare);

void heap_destroy(Heap* heap, bool free_elements);

bool heap_insert(Heap* heap, const void* element);

bool heap_extract(Heap* heap, void* out);

bool heap_peek(const Heap* heap, void* out);

size_t heap_size(const Heap* heap);

bool heap_is_empty(const Heap* heap);

int int_min_compare(const void* a, const void* b);

int int_max_compare(const void* a, const void* b);

#endif