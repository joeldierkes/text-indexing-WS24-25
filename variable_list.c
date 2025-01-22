#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "specific_trie.h"
#include "priority_queue.h"

typedef struct Node {
  char element;

  struct trie_tree *child;
} Node;

int compare_nodes(void *n1, void *n2) {
  return ((Node*)n2)->element - ((Node*)n1)->element;
}

void free_node(void **n) {
  free_trie_specific(&(*((Node** )n))->child);
  free(*n);
  *n = NULL;
}

// Contains references to all children. If the pointer is NULL it does not
// exist.
struct trie_tree {
  PriorityQueue *pq;

  // Wether this specific node is the end result of an insert.
  bool is_terminal;
};

void init_specific(struct trie_tree **root) {
  *root = calloc(1, sizeof(struct trie_tree));
  priority_queue_init(&((*root)->pq), compare_nodes, free_node);
};

bool insert_specific(struct trie_tree *root, struct trie_tree **child, char *c,
                     void (*init)(struct trie_tree **)) {
  if (priority_queue_contains(root->pq, c)) {
    *child = ((struct Node*) priority_queue_get(root->pq, c))->child;
    return false;
  } else {
    init(child);
    Node *tmp_node = malloc(sizeof(struct Node));
    tmp_node->element = *c;
    tmp_node->child = *child;
    priority_queue_insert(root->pq, tmp_node);
    return true;
  }
}

void delete_empty_child(struct trie_tree *root, char *element) {
  Node tmp_node;
  tmp_node.element = *element;
  Node *node = priority_queue_get(root->pq, &tmp_node);

  assert(node);
  // assert(node->child == NULL);

  priority_queue_delete(root->pq, &tmp_node);
}

size_t number_children(struct trie_tree *root) {
  return priority_queue_size(root->pq);
}

void get_specific(struct trie_tree *root, struct trie_tree **child,
    char *element) {
  Node tmp_node;
  tmp_node.element = *element;
  Node *node = priority_queue_get(root->pq, &tmp_node);
  if(!node) *child = NULL;
  else *child = node->child;
}

void get_children(struct trie_tree *root, struct trie_tree **children) {
  // TODO
  // Note: `children` is already initialized with the right size.
  assert(false && "not implemented");
}

void get_labels(struct trie_tree *root, char *labels) {
  // TODO
  // Note: `labels` is already initialized with the right size.
  assert(false && "not implemented");
}

bool is_terminal(struct trie_tree *root) { return root->is_terminal; }

void set_terminal(struct trie_tree *root) { root->is_terminal = true; }

void unset_terminal(struct trie_tree *root) { root->is_terminal = false; }

void free_trie_specific(struct trie_tree **root) {
  priority_queue_free(&((*root)->pq));

  free(*root);
  *root = NULL;
}
