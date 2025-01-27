#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "specific_trie.h"

#define HASH_SIZE 8

typedef struct HashEntry {
    char key;
    struct trie_tree *child;
    struct HashEntry *next;
} HashEntry;

struct trie_tree {
    HashEntry **table;
    bool is_terminal;
};

void hashmap_init_specific(struct trie_tree **root) {
    *root = calloc(1, sizeof(struct trie_tree));
    (*root)->table = calloc(HASH_SIZE, sizeof(HashEntry*));
    (*root)->is_terminal = false;
}

bool hashmap_insert_specific(struct trie_tree *root, struct trie_tree **child, char *c,
                     void (*init)(struct trie_tree **)) {
    unsigned char ch = *c;
    int index = ch % HASH_SIZE;
    HashEntry *entry = root->table[index];
    while (entry) {
        if (entry->key == ch) {
            *child = entry->child;
            return false;
        }
        entry = entry->next;
    }
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    new_entry->key = ch;
    init(child);
    new_entry->child = *child;
    new_entry->next = root->table[index];
    root->table[index] = new_entry;
    return true;
}

size_t hashmap_number_children(struct trie_tree *root) {
    size_t count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = root->table[i];
        while (entry) {
            count++;
            entry = entry->next;
        }
    }
    return count;
}

void hashmap_free_trie_specific(struct trie_tree **root) {
    if (!*root) return;

    // Free all entries in the hash table
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = (*root)->table[i];
        while (entry) {
            HashEntry *next = entry->next;
            hashmap_free_trie_specific(&entry->child);
            // Free the current entry
            free(entry);
            entry = next;
        }
    }

    free((*root)->table);
    free(*root);
    *root = NULL;
}


void hashmap_delete_empty_child(struct trie_tree *root, char *element) {
    unsigned char ch = *element;
    int index = ch % HASH_SIZE;
    HashEntry **prev_ptr = &root->table[index];
    HashEntry *current = *prev_ptr;
    while (current) {
        if (current->key == ch) {
            *prev_ptr = current->next;
            hashmap_free_trie_specific(&current->child);
            free(current);
            return;
        }
        prev_ptr = &current->next;
        current = current->next;
    }
    assert(false);
}

void hashmap_get_specific(struct trie_tree *root, struct trie_tree **child, char *element) {
    unsigned char ch = *element;
    int index = ch % HASH_SIZE;
    HashEntry *entry = root->table[index];
    while (entry) {
        if (entry->key == ch) {
            *child = entry->child;
            return;
        }
        entry = entry->next;
    }
    *child = NULL;
}

void hashmap_get_children(struct trie_tree *root, struct trie_tree **children) {
    size_t idx = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = root->table[i];
        while (entry) {
            children[idx++] = entry->child;
            entry = entry->next;
        }
    }
}

void hashmap_get_labels(struct trie_tree *root, char *labels) {
    size_t idx = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = root->table[i];
        while (entry) {
            labels[idx++] = entry->key;
            entry = entry->next;
        }
    }
}

bool hashmap_is_terminal(struct trie_tree *root) { return root->is_terminal; }

void hashmap_set_terminal(struct trie_tree *root) { root->is_terminal = true; }

void hashmap_unset_terminal(struct trie_tree *root) { root->is_terminal = false; }

size_t hashmap_get_size(struct trie_tree *root) {
    size_t size = sizeof(struct trie_tree) + HASH_SIZE * sizeof(HashEntry*);
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = root->table[i];
        while (entry) {
            size += sizeof(HashEntry);
            entry = entry->next;
        }
    }
    return size;
}

extern struct specific_trie_implementation SPECIFIC_IMPLEMENTATIONS[3];

void register_hash_map() {
    struct specific_trie_implementation impl = {
        .init_specific = hashmap_init_specific,
        .insert_specific = hashmap_insert_specific,
        .delete_empty_child = hashmap_delete_empty_child,
        .number_children = hashmap_number_children,
        .get_specific = hashmap_get_specific,
        .get_children = hashmap_get_children,
        .get_labels = hashmap_get_labels,
        .set_terminal = hashmap_set_terminal,
        .unset_terminal = hashmap_unset_terminal,
        .is_terminal = hashmap_is_terminal,
        .get_size = hashmap_get_size,
        .free_trie_specific = hashmap_free_trie_specific
    };
    SPECIFIC_IMPLEMENTATIONS[2] = impl;
}
