#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stdbool.h>

typedef int COMP_FUNC(void*, void*);
typedef struct PriorityQueue PriorityQueue;

// Initializes the PQ.
//
// The second argument is the comparison function.
bool priority_queue_init(PriorityQueue**, COMP_FUNC);

// Inserts an element into the PQ.
bool priority_queue_insert(PriorityQueue*, void*);

// Removes the first element from the PQ and sets the corresponding value.
bool priority_queue_delete(PriorityQueue*, void*);

// Finds an element in the PQ.
bool priority_queue_contains(PriorityQueue*, void*);

// Returns true if the PQ is empty, false otherwise.
bool priority_queue_is_empty(PriorityQueue *root);

// Frees the PQ.
bool priority_queue_free(PriorityQueue**);

#endif
