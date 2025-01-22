
#include "priority_queue.h"

#include <assert.h>
#include <stdlib.h>

typedef struct PQNode {
  struct PQNode *next;
  void *element;
} PQNode;

typedef struct PriorityQueue {
  COMP_FUNC *comp_func;
  FREE_FUNC *free_func;
  PQNode *head;
} PriorityQueue;


bool priority_queue_init(PriorityQueue** root, COMP_FUNC comp_func, FREE_FUNC free_func) {
  *root = (PriorityQueue*) malloc(sizeof(PriorityQueue));
  (*root)->comp_func = comp_func;
  (*root)->free_func = free_func;
  (*root)->head = NULL;
  return true;
}

void insert_element_after(PQNode *node, void * element) {
  PQNode *new = malloc(sizeof(PQNode));
  new->next = node->next;
  new->element = element;
  node->next = new;
}

bool priority_queue_insert(PriorityQueue *root, void * element) {
  PQNode *tmp_node = root->head, *prev_node = NULL;
  while(tmp_node && root->comp_func(tmp_node->element, element) > 0) {
    prev_node = tmp_node;
    tmp_node = tmp_node->next;
  }

  if(prev_node == NULL) {
    assert(root->head == NULL);
    root->head = calloc(1, sizeof(PQNode));
    root->head->element = element;
    return true;
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

void * priority_queue_get(PriorityQueue *root, void * element) {
  PQNode *tmp_node = root->head;
  while(tmp_node && root->comp_func(tmp_node->element, element) > 0) {
    tmp_node = tmp_node->next;
  }

  if (tmp_node == NULL) {
    return NULL;
  }

  if (root->comp_func(tmp_node->element, element) == 0) {
    return tmp_node->element;
  }
  return NULL;
}

void remove_element_after(PriorityQueue *root, PQNode *node) {
  PQNode *tmp = node->next;
  node->next = node->next->next;
  root->free_func(&(tmp->element));
  free(tmp);
}

bool priority_queue_delete(PriorityQueue *root, void * element) {
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

  if (prev_node == NULL) {
    root->head = root->head->next;
    root->free_func(&(tmp_node->element));
    free(tmp_node);
  } else {
    remove_element_after(root, prev_node);
  }
  return true;
}

bool priority_queue_contains(PriorityQueue *root, void * element) {
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
    remove_element_after(*root, (*root)->head);
  }

  if ((*root)->head) {
    (*root)->free_func(&((*root)->head->element));
    free((*root)->head);
  }

  free(*root);
  return true;
}

bool priority_queue_size(PriorityQueue *root) {
  size_t size = 0;
  PQNode *tmp_node = root->head;
  while (tmp_node) {
    tmp_node = tmp_node->next;
    size++;
  }
  return size;
}
