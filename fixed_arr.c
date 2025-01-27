#include <assert.h>
#include <stdlib.h>

#include "specific_trie.h"

size_t char_to_index(char c) {
  return ('a' <= c && c <= 'z') * (c - 'a') +
         ('A' <= c && c <= 'Z') * (c - 'A' + 26) +
         ('0' <= c && c <= '9') * (c - '0' + 26 + 26);
}

char index_to_char(size_t idx) {
  return (idx < 26) * (idx + 'a') +
         (26 <= idx && idx < 26 + 26) * (idx - 26 + 'A') +
         (26 + 26 <= idx && idx < 26 + 26 + 10) * (idx - 26 - 26 + '0');
}

// Contains references to all children. If the pointer is NULL it does not
// exist.
struct trie_tree {
  struct trie_tree *children[FIXED_SIZE_LEN];

  // Wether this specific node is the end result of an insert.
  bool is_terminal;
};

void fixed_init_specific(struct trie_tree **root) {
  *root = calloc(1, sizeof(struct trie_tree));
};

bool fixed_insert_specific(struct trie_tree *root, struct trie_tree **child, char *c,
                     void (*init)(struct trie_tree **)) {
  size_t idx = char_to_index(*c);

  if (root->children[idx] != NULL) {
    *child = root->children[idx];
    return false;
  } else {
    init(child);
    root->children[idx] = *child;
    return true;
  }
}

size_t fixed_number_children(struct trie_tree *root) {
  size_t res = 0;
  for (size_t i = 0; i < FIXED_SIZE_LEN; ++i) {
    if (root->children[i] != NULL) {
      res++;
    }
  }

  return res;
}

void fixed_delete_empty_child(struct trie_tree *root, char *element) {
  size_t idx = char_to_index(*element);

  // Note that this is disabled in release mode.
  assert(fixed_number_children(root->children[idx]) == 0);

  free(root->children[idx]);
  root->children[idx] = NULL;
}

void fixed_get_specific(struct trie_tree *root, struct trie_tree **child,
                  char *element) {
  size_t idx = char_to_index(*element);
  *child = root->children[idx];
}

void fixed_get_children(struct trie_tree *root, struct trie_tree **children) {
  size_t outer = 0;
  for (size_t i = 0; i < FIXED_SIZE_LEN; ++i) {
    if (root->children[i] != NULL) {
      children[outer++] = root->children[i];
    }
  }
}

void fixed_get_labels(struct trie_tree *root, char *labels) {
  size_t outer = 0;
  for (size_t i = 0; i < FIXED_SIZE_LEN; ++i) {
    if (root->children[i] != NULL) {
      labels[outer++] = index_to_char(i);
    }
  }
}

bool fixed_is_terminal(struct trie_tree *root) { return root->is_terminal; }

void fixed_set_terminal(struct trie_tree *root) { root->is_terminal = true; }

void fixed_unset_terminal(struct trie_tree *root) { root->is_terminal = false; }

size_t fixed_get_size(struct trie_tree *root) {
  return sizeof(root->is_terminal) + sizeof(root->children);
}

void fixed_free_trie_specific(struct trie_tree **root) {
  for (size_t i = 0; i < FIXED_SIZE_LEN; ++i) {
    if ((*root)->children[i] != NULL) {
      fixed_free_trie_specific(&((*root)->children[i]));
    }
  }

  free(*root);
  *root = NULL;
}

extern struct specific_trie_implementation SPECIFIC_IMPLEMENTATIONS[3];

void register_fixed_arr() {
  struct specific_trie_implementation impl = {
      .init_specific = &fixed_init_specific,
      .insert_specific = &fixed_insert_specific,
      .delete_empty_child = &fixed_delete_empty_child,
      .number_children = &fixed_number_children,
      .get_specific = &fixed_get_specific,
      .get_children = &fixed_get_children,
      .get_labels = &fixed_get_labels,
      .set_terminal = &fixed_set_terminal,
      .unset_terminal = &fixed_unset_terminal,
      .is_terminal = &fixed_is_terminal,
      .get_size = &fixed_get_size,
      .free_trie_specific = &fixed_free_trie_specific,
  };
  SPECIFIC_IMPLEMENTATIONS[0] = impl;
}
