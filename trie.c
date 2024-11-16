#include "trie.h"
#include "specific_trie.h"

#include <stdio.h>
#include <string.h>

void init(struct trie_tree **root) {
  init_specific(root);
}

bool insert(struct trie_tree *root, char *element) {
  if (*element == '\0') {
    // This is only the case if the word is empty.
    return false;
  }

  struct trie_tree *child;

  insert_specific(root, &child, *element, init);

  if (*(element + 1) == '\0') {
    // We know that this is the last character in the word, i.e., the
    // word is completly processed.

    if (is_terminal(child)) {
      // The word already exists in the trie.
      return false;
    }

    set_terminal(child);
    return true;
  }

  return insert(child, element + 1); 
}

bool contains(struct trie_tree *root, char *element) {
  if (*element == '\0') {
    // The element does not exist in the trie
    return false;
  }

  struct trie_tree *child;

  get_specific(root, &child, element);
  if (!child) {
    return false;
  }

  if (*(element + 1) == '\0' && is_terminal(child)) {
    return true;
  }

  return contains(child, element + 1);
}

bool delete_helper(struct trie_tree *root, char *element, bool *should_delete) {
  if (*element == '\0') {
    // The element does not exist in the trie
    return false;
  }

  struct trie_tree *child;

  get_specific(root, &child, element);
  if (!child) {
    // The element does not exist in the trie
    return false;
  }

  if (*(element + 1) == '\0' && is_terminal(child) && number_children(child) == 0) {
    // At this point we know that the child is _our_ leaf (this is
    // important if a suffix of our word without any children is
    // contained in the trie as well). We can safely remove it and
    // remove all parents till there is one with other children.

    *should_delete = true;
    delete_empty_child(root, element);
    return true;
  }

  if (*(element + 1) == '\0' && is_terminal(child)) {
    // This is the case if the word is a suffix of another word in the
    // trie.

    *should_delete = false;
    unset_terminal(child);
    return true;
  }

  if (delete_helper(child, element + 1, should_delete)) {
    if (!*should_delete) {
      // This is the case if there exists some other children down of
      // the current path. We don't have to do anything.
      return true;
    }

    if (is_terminal(child)) {
      // A suffix of the deleted word is contained in the subtree.

      *should_delete = false;
      return true;
    }

    if (number_children(child) != 0) {
      // Now we don't have to delete anything anymore. There exists
      // another child.
      *should_delete = false;
      return true;
    }

    // At this point we know that the search was sucessfull and we
    // have no other children.
    delete_empty_child(root, element);

    return true;
  } else {
    return false;
  }
}

bool delete(struct trie_tree *root, char *element) {
  bool should_delete = false;
  return delete_helper(root, element, &should_delete);
}

void free_trie(struct trie_tree* *root) {
  free_trie_specific(root);
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
