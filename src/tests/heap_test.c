#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "test.h"
#include "ds/heap.h"

static int test_count = 0;

void test_create_heap_success()
{
  printf("Testing creation of a heap with valid parameters...\n");
  Heap* heap = heap_create(10, sizeof(int), int_min_compare);
  TEST_ASSERT(heap != NULL);
  TEST_ASSERT(heap_size(heap) == 0);
  TEST_ASSERT(heap_is_empty(heap) == true);
  heap_destroy(heap, true);
}

void test_create_heap_failed()
{
  printf("Testing creation of a heap with invalid parameters...\n");
  Heap* heap = heap_create(0, sizeof(int), int_min_compare);
  TEST_ASSERT(heap == NULL);
}

void test_create_int_max_heap_success() {
  printf("Testing creation of a max heap with integer element...\n");
  Heap* max_heap = heap_create(10, sizeof(int), int_max_compare);

  int values[] = {5, 3, 8, 1, 2};
  for (int i = 0; i < 5; i++) {
    TEST_ASSERT(heap_insert(max_heap, &values[i]));
  }

  qsort(values, 5, sizeof(int), int_max_compare);
  for (int i = 0; i < 5; i++) {
    int heap_value;
    TEST_ASSERT(heap_extract(max_heap, &heap_value));
    TEST_ASSERT(heap_value == values[i]);
  }

  heap_destroy(max_heap, true);
}

void test_create_int_min_heap_success() {
  printf("Testing creation of a min heap with integer element...\n");
  Heap* min_heap = heap_create(10, sizeof(int), int_min_compare);

  int values[] = {5, 3, 8, 1, 2};
  for (int i = 0; i < 5; i++) {
    TEST_ASSERT(heap_insert(min_heap, &values[i]));
  }

  qsort(values, 5, sizeof(int), int_min_compare);
  for (int i = 0; i < 5; i++) {
    int heap_value;
    TEST_ASSERT(heap_extract(min_heap, &heap_value));
    TEST_ASSERT(heap_value == values[i]);
  }

  heap_destroy(min_heap, true);
}

void test_heap_peek_success() {
  printf("Testing heap peek operation...\n");
  Heap* min_heap = heap_create(10, sizeof(int), int_min_compare);

  int values[] = {5, 3, 8, 1, 2};
  for (int i = 0; i < 5; i++) {
    heap_insert(min_heap, &values[i]);
  }

  qsort(values, 5, sizeof(int), int_max_compare);

  int peek_val;
  TEST_ASSERT(heap_peek(min_heap, &peek_val));
  TEST_ASSERT(peek_val == values[0]);
  TEST_ASSERT(heap_size(min_heap) == 5);
  heap_destroy(min_heap, true);
}

typedef struct
{
  int id;
  char name[20];
} Person;

int person_compare_by_id(const void* a, const void* b)
{
  Person* p1 = (Person*)a;
  Person* p2 = (Person*)b;

  if (p1->id < p2->id) return -1;
  if (p1->id > p2->id) return 1;
  return 0;
}

void test_custom_type()
{
  Heap* person_heap = heap_create(10, sizeof(Person), person_compare_by_id);
  TEST_ASSERT(person_heap != NULL);

  Person people[] =
  {
    {1, "Dave"},
    {5, "Delya"},
    {2, "Evelyn"}
  };

  for (int i = 0; i < 3; i++)
  {
    TEST_ASSERT(heap_insert(person_heap, &people[i]));
  }

  qsort(people, 3, sizeof(Person), person_compare_by_id);

  Person extracted;
  TEST_ASSERT(heap_peek(person_heap, &extracted));
  TEST_ASSERT(extracted.id == people[0].id);
  TEST_ASSERT(strcmp(extracted.name, people[0].name) == 0);
  heap_destroy(person_heap, true);
}

int main() {
  printf("Running heap unit tests...\n\n");

  test_create_heap_success();
  test_create_heap_failed();
  test_create_int_max_heap_success();
  test_create_int_min_heap_success();
  test_custom_type();

  printf("\nTest Summary: %d assertions passed\n", test_count);

  return EXIT_SUCCESS;
}
