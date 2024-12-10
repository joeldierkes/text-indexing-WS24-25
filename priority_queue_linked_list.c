
#include "priority_queue.h"

#include <assert.h>
#include <stdlib.h>

typedef struct PQNode {
  struct PQNode *next;
  void* element;
} PQNode;

typedef struct PriorityQueue {
  COMP_FUNC *comp_func;
  PQNode *head;
} PriorityQueue;


bool priority_queue_init(PriorityQueue** root, COMP_FUNC comp_func) {
  *root = (PriorityQueue*) malloc(sizeof(PriorityQueue));
  (*root)->comp_func = comp_func;
  (*root)->head = NULL;
  return true;
}

void insert_element_after(PQNode *node, void *element) {
  PQNode *new = malloc(sizeof(PQNode));
  new->next = node->next;
  new->element = element;
  node->next = new;
}

bool priority_queue_insert(PriorityQueue *root, void *element) {
  PQNode *tmp_node = root->head, *prev_node = NULL;
  while(tmp_node && root->comp_func(tmp_node->element, element) > 0) {
    prev_node = tmp_node;
    tmp_node = tmp_node->next;
  }

  if(prev_node == NULL) {
    assert(root->head == NULL);
    root->head = calloc(1, sizeof(PQNode));
    root->head->element = element;
  }

  if (tmp_node == NULL) {
    insert_element_after(prev_node, element);
    return true;
  }

  if (root->comp_func(tmp_node->element, element) == 0) {
    return false;
  }

  insert_element_after(tmp_node, element);
  return true;
}

void remove_element_after(PQNode *node) {
  PQNode *tmp = node->next;
  node->next = node->next->next;
  // free(tmp->element);
  free(tmp);
}

bool priority_queue_delete(PriorityQueue *root, void *element) {
  PQNode *tmp_node = root->head, *prev_node = NULL;
  while(tmp_node && root->comp_func(tmp_node->element, element) > 0) {
    prev_node = tmp_node;
    tmp_node = tmp_node->next;
  }

  if (tmp_node == NULL) {
    return false;
  }

  if (root->comp_func(tmp_node->element, element) != 0) {
    return false;
  }

  remove_element_after(prev_node);
  return true;
}

bool priority_queue_contains(PriorityQueue *root, void *element) {
  PQNode *tmp_node = root->head;
  while(tmp_node && root->comp_func(tmp_node->element, element) > 0) {
    tmp_node = tmp_node->next;
  }

  if (tmp_node == NULL) {
    return false;
  }

  if (root->comp_func(tmp_node->element, element) == 0) {
    return true;
  }
  return false;
}

bool priority_queue_is_empty(PriorityQueue *root) {
  return root->head == NULL;
}

// Frees the PQ.
bool priority_queue_free(PriorityQueue **root) {
  while ((*root)->head && (*root)->head->next) {
    remove_element_after((*root)->head);
  }

  if ((*root)->head) {
    free((*root)->head);
  }

  free(*root);
  return true;
}
