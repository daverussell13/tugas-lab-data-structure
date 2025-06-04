#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds/heap.h"

#define MAX_NAME_LENGTH 50

typedef struct
{
  int priority;
  char name[MAX_NAME_LENGTH];
} Patient;

int patient_compare(const void* a, const void* b)
{
  const Patient* patient_a = (const Patient*)a;
  const Patient* patient_b = (const Patient*)b;

  if (patient_a->priority < patient_b->priority) return -1;
  if (patient_a->priority > patient_b->priority) return 1;

  return strcmp(patient_a->name, patient_b->name);
}

void display_menu()
{
  printf("\n===== HOSPITAL QUEUE SYSTEM =====\n");
  printf("1. Add Patient\n");
  printf("2. Process Next Patient\n");
  printf("3. View Next Patient\n");
  printf("4. Display Queue\n");
  printf("5. Exit\n");
  printf("Enter your choice: ");
}

void add_patient(Heap* queue)
{
  Patient new_patient;

  printf("\nEnter patient name: ");
  scanf(" %[^\n]s", new_patient.name);

  printf("Enter priority (1-5, lower number = higher priority): ");
  scanf("%d", &new_patient.priority);

  if (new_patient.priority < 1 || new_patient.priority > 5)
  {
    printf("Invalid priority! Must be between 1 and 5.\n");
    return;
  }

  if (heap_insert(queue, &new_patient))
  {
    printf("Patient %s added with priority %d\n", new_patient.name, new_patient.priority);
  }
  else
  {
    printf("Failed to add patient to the queue.\n");
  }
}

void process_patient(Heap* queue)
{
  if (heap_is_empty(queue))
  {
    printf("\nQueue is empty. No patients to process.\n");
    return;
  }

  Patient next_patient;

  if (heap_extract(queue, &next_patient))
  {
    printf("\nProcessing patient: %s (Priority: %d)\n", next_patient.name, next_patient.priority);
  }
  else
  {
    printf("\nFailed to process patient.\n");
  }
}

void view_next_patient(Heap* queue)
{
  if (heap_is_empty(queue))
  {
    printf("\nQueue is empty. No patients in line.\n");
    return;
  }

  Patient next_patient;

  if (heap_peek(queue, &next_patient))
  {
    printf("\nNext patient: %s (Priority: %d)\n", next_patient.name, next_patient.priority);
  }
  else
  {
    printf("\nFailed to view next patient.\n");
  }
}

void display_queue(Heap* queue)
{
  if (heap_is_empty(queue))
  {
    printf("\nQueue is empty. No patients in line.\n");
    return;
  }

  printf("\nCurrent Queue (Total patients: %zu):\n", heap_size(queue));
  printf("Note: This view doesn't show the exact order of processing.\n");

  Heap* temp_heap = heap_create(queue->capacity, sizeof(Patient), patient_compare);
  Patient patient;
  size_t count = 0;

  while (!heap_is_empty(queue))
  {
    heap_extract(queue, &patient);
    printf("%zu. %s (Priority: %d)\n", ++count, patient.name, patient.priority);
    heap_insert(temp_heap, &patient);
  }

  while (!heap_is_empty(temp_heap))
  {
    heap_extract(temp_heap, &patient);
    heap_insert(queue, &patient);
  }

  heap_destroy(temp_heap, true);
}

int main()
{
  Heap* patient_queue = heap_create(10, sizeof(Patient), patient_compare);

  if (patient_queue == NULL)
  {
    printf("Failed to create patient queue. Exiting...\n");
    return EXIT_FAILURE;
  }

  int choice;
  bool running = true;

  printf("Welcome to Hospital Queue Management System\n");

  while (running)
  {
    display_menu();
    scanf("%d", &choice);

    switch (choice)
    {
      case 1:
        add_patient(patient_queue);
        break;

      case 2:
        process_patient(patient_queue);
        break;

      case 3:
        view_next_patient(patient_queue);
        break;

      case 4:
        display_queue(patient_queue);
        break;

      case 5:
        printf("\nExiting Hospital Queue Management System. Goodbye!\n");
        running = false;
        break;

      default:
        printf("\nInvalid choice. Please try again.\n");
    }
  }

  heap_destroy(patient_queue, true);

  return EXIT_SUCCESS;
}