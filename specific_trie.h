#include <stdbool.h>
#include <stdlib.h>

#define FIXED_SIZE_LEN (26 + 26 + 10)

struct trie_tree;

void init_specific(struct trie_tree **);

// Inserts a character into the trie.
//
// Returns `true` if the character got inserted and a new node was
// created, `false` otherwise.
//
// Params:
//   - `parent`: The parent node to insert the character.
//   - `child`: A reference to the child node. This will be updated by this
//              function.
//   - `c`: The character to insert.
//   - `init`: An initializer function for the trie node.
bool insert_specific(struct trie_tree *parent, struct trie_tree **child, char c,
                     void (*init)(struct trie_tree **));

void delete_empty_child(struct trie_tree *, char *);

size_t number_children(struct trie_tree *);
void get_specific(struct trie_tree *, struct trie_tree **, char *);
void get_children(struct trie_tree *, struct trie_tree **);
void get_labels(struct trie_tree *, char *);
void set_terminal(struct trie_tree *);
void unset_terminal(struct trie_tree *);
bool is_terminal(struct trie_tree *);
void free_trie_specific(struct trie_tree **);
