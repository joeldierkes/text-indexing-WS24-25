
#include <stdbool.h>

struct trie_tree;

// Initialize the given trie. The internal data is specified by the concrete implementation.
void init(struct trie_tree**);

// Inserts an element in the Trie. Returns `true` if inserted successfully. `false` otherwise.
bool insert(struct trie_tree*, char*);
// Checks if an element is in the Trie. Returns `true` if found. `false` otherwise.
bool contains(struct trie_tree*, char*);
// Deletes an element from the Trie. Returns `true` if deleted successfully. `false` otherwise.
bool delete(struct trie_tree*, char*);

// Prints the DOT representation of the Trie to stdout. This is useful for debugging.
void print_dot(struct trie_tree*);


// This function inserts multiple word in a `trie_tree` from a given string.
// The string has to contain one word per line, with each word ending in '\0'.
void insert_multiple(struct trie_tree*, char*);
