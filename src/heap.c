#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds/heap.h"

#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

static void heap_swap(Heap* heap, size_t i, size_t j)
{
  void* temp = heap->array[i];
  heap->array[i] = heap->array[j];
  heap->array[j] = temp;
}

static void heapify_up(Heap* heap, size_t index)
{
  while (index > 0)
  {
    size_t parent = PARENT(index);
    if (heap->compare(heap->array[index], heap->array[parent]) < 0)
    {
      heap_swap(heap, index, parent);
      index = parent;
    }
    else
    {
      break;
    }
  }
}

static void heapify_down(Heap* heap, size_t index)
{
  size_t smallest_or_largest = index;
  size_t left = LEFT(index);
  size_t right = RIGHT(index);

  if (left < heap->size &&
    heap->compare(heap->array[left], heap->array[smallest_or_largest]) < 0)
  {
    smallest_or_largest = left;
  }

  if (right < heap->size &&
    heap->compare(heap->array[right], heap->array[smallest_or_largest]) < 0)
  {
    smallest_or_largest = right;
  }

  if (smallest_or_largest != index)
  {
    heap_swap(heap, index, smallest_or_largest);
    heapify_down(heap, smallest_or_largest);
  }
}

Heap* heap_create(size_t initial_capacity, size_t element_size, HeapCompareFunc compare)
{
  if (initial_capacity == 0 || element_size == 0 || compare == NULL)
  {
    return NULL;
  }

  Heap* heap = (Heap*)malloc(sizeof(Heap));
  if (heap == NULL)
  {
    return NULL;
  }

  heap->array = (void**)malloc(initial_capacity * sizeof(void*));
  if (heap->array == NULL)
  {
    free(heap);
    return NULL;
  }

  heap->capacity = initial_capacity;
  heap->size = 0;
  heap->element_size = element_size;
  heap->compare = compare;

  return heap;
}

void heap_destroy(Heap* heap, bool free_elements)
{
  if (heap == NULL)
  {
    return;
  }

  if (free_elements)
  {
    for (size_t i = 0; i < heap->size; i++)
    {
      free(heap->array[i]);
    }
  }

  free(heap->array);
  free(heap);
}

bool heap_insert(Heap* heap, const void* element)
{
  if (heap == NULL || element == NULL)
  {
    return false;
  }

  if (heap->size >= heap->capacity)
  {
    size_t new_capacity = heap->capacity * 2;
    void** new_array = (void**)realloc(heap->array, new_capacity * sizeof(void*));
    if (new_array == NULL)
    {
      return false;
    }
    heap->array = new_array;
    heap->capacity = new_capacity;
  }

  void* element_copy = malloc(heap->element_size);
  if (element_copy == NULL)
  {
    return false;
  }
  memcpy(element_copy, element, heap->element_size);

  heap->array[heap->size] = element_copy;
  heapify_up(heap, heap->size);
  heap->size++;

  return true;
}

bool heap_extract(Heap* heap, void* out)
{
  if (heap == NULL || out == NULL || heap->size == 0)
  {
    return false;
  }

  memcpy(out, heap->array[0], heap->element_size);

  free(heap->array[0]);

  if (heap->size > 1)
  {
    heap->array[0] = heap->array[heap->size - 1];
  }

  heap->size--;

  if (heap->size > 0)
  {
    heapify_down(heap, 0);
  }

  return true;
}

bool heap_peek(const Heap* heap, void* out)
{
  if (heap == NULL || out == NULL || heap->size == 0)
  {
    return false;
  }

  memcpy(out, heap->array[0], heap->element_size);
  return true;
}

size_t heap_size(const Heap* heap)
{
  return heap ? heap->size : 0;
}

bool heap_is_empty(const Heap* heap)
{
  return heap ? heap->size == 0 : true;
}

int int_min_compare(const void* a, const void* b)
{
  int val_a = *((const int*)a);
  int val_b = *((const int*)b);

  if (val_a < val_b) return -1;
  if (val_a > val_b) return 1;
  return 0;
}

int int_max_compare(const void* a, const void* b)
{
  int val_a = *((const int*)a);
  int val_b = *((const int*)b);

  if (val_a > val_b) return -1;
  if (val_a < val_b) return 1;
  return 0;
}