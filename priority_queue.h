#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stdbool.h>

typedef int COMP_FUNC(void *, void *);
typedef void FREE_FUNC(void **);
typedef struct PriorityQueue PriorityQueue;

// Initializes the PQ.
//
// The second argument is the comparison function.
bool priority_queue_init(PriorityQueue**, COMP_FUNC, FREE_FUNC);

// Finds an element in the PQ and returns it.
void *priority_queue_get(PriorityQueue*, void *);

// Inserts an element into the PQ.
bool priority_queue_insert(PriorityQueue*, void *);

// Removes the first element from the PQ and sets the corresponding value.
bool priority_queue_delete(PriorityQueue*, void *);

// Finds an element in the PQ.
bool priority_queue_contains(PriorityQueue*, void *);

// Returns true if the PQ is empty, false otherwise.
bool priority_queue_is_empty(PriorityQueue *root);

// Frees the PQ.
bool priority_queue_free(PriorityQueue**);

// Number of elements in the PQ.
bool priority_queue_size(PriorityQueue*);

#endif
