#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "trie.h"


struct specific_trie_implementation SPECIFIC_IMPLEMENTATIONS[3];
struct specific_trie_implementation SPECIFIC_IMPLEMENTATION;

typedef void(action_callback)(struct trie_tree *, bool status);

static char **RESULT;

void save_result_callback(struct trie_tree *_, bool status) {
  static int CURRENT_RESULT = 0;
  RESULT[CURRENT_RESULT++] = status ? "true" : "false";
}

void print_status_callback(struct trie_tree *_, bool status) {
  printf("%s\n", status ? "true" : "false");
}

static void die(int line_number, const char *format, ...) {
  va_list vargs;
  va_start(vargs, format);
  fprintf(stderr, "%d: ", line_number);
  vfprintf(stderr, format, vargs);
  fprintf(stderr, ".\n");
  va_end(vargs);
  exit(1);
}

// Inserts the words given in `s` into `root`.
//
// `s` contains one word per line, with each word ending in
// '\0'. (Don't ask me why.)
//
// Note that the last word must also contain a new line character at
// the end!
void insert_multiple(struct trie_tree *root, char *s) {
  while (true) {
    insert(root, s);
    while (*(s++) != '\0')
      ;
    if (*s == '\n') {
      s++;
    }
    if (*s == '\0') {
      break;
    }
  }
}

// Executes the queries given by `queries`.
//
// The queries follow a specific (probably ill defined by me) grammar:
// ```
// <queries> ::= <query> | <query>\n<queries>
// <query>   ::= <word><null><command> | <word><null> <command>
// <word>    ::= <symbol> | <word><symbol>
// <symbol>  ::= a-z | A-Z | 0-9
// <command> ::= i | c | d
// <null>    ::= \0
// ```
//
// ## Note
// Dollar signs (`$`) have previously been converted to null characters.
//
// A command either returns true or false:
// - insert (i): Returns true if the element did not exist already and
//               got inserted, false otherwise.
// - contains (c): Returns true if the element is contained in the
//                 trie, false otherwise.
// - delete (d): Returns true if the element existed in the trie and
//               got deleted, false otherwise.
//
// Note: This function does not check the grammar, it assumes that
// `queries` is well defined.
void execute_queries(struct trie_tree *root, char *queries,
                     action_callback callback) {
  while (true) {
    char *query = queries;
    bool (*action)(struct trie_tree *, char *);

    while (*(queries++) != '\0')
      ;

    if (*queries == ' ')
      queries++;

    switch (*queries) {
    case 'i':
      action = insert;
      break;
    case 'c':
      action = contains;
      break;
    case 'd':
      action = delete;
      break;
    default:
      action = NULL;
      die(__LINE__, "Ill definined action! %c", queries);
    }

    callback(root, action(root, query));

    queries = queries + 2;
    if (*queries == '\0') {
      break;
    }
  }
}

// Reads the entrie file content to memory.
void get_file_content(char *filename, char **content, size_t *bytes_read) {
  // Open in binary mode since the file contains null characters.
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    die(__LINE__, "Error opening file");
  }

  // Move to the end to determine the file size
  fseek(file, 0, SEEK_END);
  *bytes_read = ftell(file);
  rewind(file);

  // Allocate memory to hold the entire file
  *content = malloc((*bytes_read + 1) * sizeof(char));
  if (*content == NULL) {
    fclose(file);
    die(__LINE__, "Memory allocation failed");
  }

  size_t bytesRead = fread(*content, 1, *bytes_read, file);
  if (bytesRead != *bytes_read) {
    free(*content);
    fclose(file);
    die(__LINE__, "File read error!");
  }
  fclose(file);

  (*content)[*bytes_read] = '\0';
}

void convert_dollar_to_null(char *string, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (string[i] == '$') {
      string[i] = '\0';
    }
  }
}


int get_variante(int *argc, char **argv) {
  int ret = 1;

  for (int i = 1; i < *argc; i++) {
    if (strstr(argv[i], "-variante=")) {
      ret = atoi(argv[i] + strlen("-variante="));
      *argc = *argc - 1;
      for (int j = i; j < *argc; j++) {
        argv[j] = argv[j + 1];
      }
    }
  }

  return ret;
}

int count_queries(const char *string, size_t len) {
  int ret = 0;
  for (int i = 0; i < len; i++) {
    if (string[i] == '\0') {
      ret++;
    }
  }
  return ret;
}

const char *concat(char* s1, char* s2){
  char *ns = malloc(strlen(s1) + strlen(s2) + 1);
  ns[0] = '\0';
  strcat(ns, s1);
  strcat(ns, s2);
  return ns;
}

const char *VARIANT_NAMES[] = {
  "fixed_size_array",
  "variable_sized_array",
  "hash_map"
};

const char *USAGE_INFORMATION =
    "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE QUERY_FILE\n"
    "\n"
    "The main entry point for the text indexing exercise 2024/25.\n"
    "\n"
    "options:\n"
    "-variante   VARIANTE,  selects the variant to run, in [1, 3].\n"
    "-t          TASK_MODE,  displays the execution status of each word in the query. "
    "One status per line. DEFAULT\n"
    "-p          PRINT_MODE,  prints the result of each command. One per line.\n"
    "-d          DOT_MODE,   displays the generated trie in a DOT readable format.\n"
    "-u          DUMP_MODE,  dumps the trie one word per line.\n";

int main(int argc, char **argv) {
  register_fixed_arr();
  register_variable_list();
  register_hash_map();

  // `getopt` can only work with shortopts or options that are provided via `--`.
  // This is a nasty hack to get the variante.
  int variante = get_variante(&argc, argv);
  SPECIFIC_IMPLEMENTATION = SPECIFIC_IMPLEMENTATIONS[variante - 1];

  int opt;
  enum { TASK_MODE, DOT_MODE, DUMP_MODE, PRINT_MODE } mode = TASK_MODE;
  while ((opt = getopt(argc, argv, "tdup")) != -1) {
    switch (opt) {
    case 't':
      mode = TASK_MODE;
      break;
    case 'd':
      mode = DOT_MODE;
      break;
      case 'u':
        mode = DUMP_MODE;
      break;
      case 'p':
        mode = PRINT_MODE;
      break;
      default:;
    }
  }

  if ((argc - optind) != 2) {
    printf("%s", USAGE_INFORMATION);
    exit(1);
  }

  size_t fsize;
  char *fbuffer;
  get_file_content(argv[optind], &fbuffer, &fsize);
  convert_dollar_to_null(fbuffer, fsize);
  size_t isize;
  char *queries;
  get_file_content(argv[optind + 1], &queries, &isize);
  convert_dollar_to_null(queries, isize);

  int number_queries = count_queries(queries, isize);
  RESULT = malloc(number_queries * sizeof(char *));

  struct trie_tree *root;

  clock_t construction_begin = clock();
  init(&root);
  insert_multiple(root, fbuffer);
  double construction_time = ((double)(clock() - construction_begin))/CLOCKS_PER_SEC * 1000;
  free(fbuffer);
  double construction_memory = (1.0 * get_size(root)) / 1024. / 1024.;

  clock_t query_begin = clock();
  execute_queries(root, queries,
                  mode == PRINT_MODE ? print_status_callback : save_result_callback);
  double query_time = ((double)(clock() - query_begin))/CLOCKS_PER_SEC * 1000;
  free(queries);

  printf("RESULT name=\"Joel Dierkes\" trie_variant=%s trie_construction_time=%f "
    "trie_construction_memory=%f query_time=%f\n", VARIANT_NAMES[variante - 1], construction_time, construction_memory, query_time);

  switch (mode) {
    case DOT_MODE:
      print_dot(root);
      break;
    case DUMP_MODE:
      die(__LINE__, "Not implemented yet");
      break;
    case TASK_MODE:
      FILE *fptr = fopen(concat("result_", argv[optind]), "w");
      for (size_t i = 0; i < number_queries; i++) {
        fprintf(fptr, concat(RESULT[i], "\n"));
      }
    fclose(fptr);
    default:
      break;
  }

  free_trie(&root);
}
