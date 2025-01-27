#ifndef _SPECIFIC_TRIE_H_
#define _SPECIFIC_TRIE_H_

#include <stdbool.h>
#include <stdlib.h>

#define FIXED_SIZE_LEN (26 + 26 + 10)

struct trie_tree;

struct specific_trie_implementation {
    void (*init_specific)(struct trie_tree **);
    bool (*insert_specific)(struct trie_tree *parent, struct trie_tree **child, char *c,
                         void (*init)(struct trie_tree **));

    void (*delete_empty_child)(struct trie_tree *, char *);
    size_t (*number_children)(struct trie_tree *);
    void (*get_specific)(struct trie_tree *, struct trie_tree **, char *);
    void (*get_children)(struct trie_tree *, struct trie_tree **);
    void (*get_labels)(struct trie_tree *, char *);
    void (*set_terminal)(struct trie_tree *);
    void (*unset_terminal)(struct trie_tree *);
    bool (*is_terminal)(struct trie_tree *);
    void (*free_trie_specific)(struct trie_tree **);
};

void register_fixed_arr();
void register_variable_list();

#endif  // _SPECIFIC_TRIE_H_
