#include <stdlib.h>
#include <stdbool.h>

#define FIXED_SIZE_LEN (26 + 26 + 10)

struct trie_tree;

void init_specific(struct trie_tree**);
bool insert_specific(struct trie_tree*, struct trie_tree**, char*, void(struct trie_tree**));
void delete_empty_child(struct trie_tree*, char*);

size_t number_children(struct trie_tree*);
void get_specific(struct trie_tree*, struct trie_tree**, char*);
void get_children(struct trie_tree*, struct trie_tree**);
void get_labels(struct trie_tree*, char*);
void set_terminal(struct trie_tree*);
void unset_terminal(struct trie_tree*);
bool is_terminal(struct trie_tree*);
