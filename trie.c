#include "trie.h"
#include "specific_trie.h"

#include <stdio.h>
#include <string.h>

void init(struct trie_tree **root) {
  init_specific(root);
}

bool insert(struct trie_tree *root, char *element) {
  if (*element == '\0') {
    return false;
  }

  struct trie_tree *child;

  if (insert_specific(root, &child, element, init)) {
    return true;
  }

  return insert(child, element + 1); 
}

bool contains(struct trie_tree *root, char *element) {
  if (*element == '\0') {
    return false;
  }

  struct trie_tree *child;

  get_specific(root, &child, element);
  if (!child) {
    return false;
  }

  if (is_terminal(child)) {
    return true;
  }

  return contains(child, element + 1); 

}

bool delete(struct trie_tree *root, char *element) {
  return false;
}

void print_dot_helper(struct trie_tree *root, char label, char *prefix) {
  printf("%s [label=\"%c\" shape=%s];\n", prefix, label, is_terminal(root) ? "doublecircle" : "circle");

  size_t prefix_len = strlen(prefix);
  char *new_prefix = calloc(prefix_len + 1, sizeof(char));
  strcpy(new_prefix, prefix);

  size_t n = number_children(root);
  struct trie_tree **children = calloc(n, sizeof(struct trie_tree*));
  get_children(root, children);
  char *labels = calloc(n + 1, sizeof(char));
  get_labels(root, labels);

  for(size_t i = 0; i < n; ++i) {
    new_prefix[prefix_len] = labels[i];
    printf("%s -> %s;\n", prefix, new_prefix);
    print_dot_helper(children[i], labels[i], new_prefix);
  }

  free(labels);
  free(children);
  free(new_prefix);
}

void print_dot(struct trie_tree *root) {
  printf("digraph trie_tree {\n");
  print_dot_helper(root, '_', "a");
  printf("}\n");
}
