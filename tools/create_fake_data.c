//usr/bin/env tcc -run "$0" "$@" ; exit $?

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_INDEXES (26 + 26 + 10)

char index_to_char(size_t idx) {
  return (idx < 26) * (idx + 'a')
    + (26 <= idx && idx < 26 + 26) * (idx - 26 + 'A')
    + (26 + 26 <= idx && idx >= 26 + 26) * (idx - 26 - 26 + '0');
}

char *generate_random_word(unsigned int n) {
  char *ret = malloc((n + 1) * sizeof(char));

  size_t i = 0;
  for(; i < n; ++i) {
    ret[i] = index_to_char(rand() % NUMBER_INDEXES);
  }
  ret[i] = '\0';

  return ret;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: ./create_fake_data.c WORD_LEN NUMBER_WORDS\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  int number_words = atoi(argv[2]);

  char *words = malloc((n * number_words + 2 * number_words + 1) * sizeof(char));
  for(int i = 0; i < number_words; ++i) {
    char *tmp = generate_random_word(n);
    strcpy(words + (n + 2) * i, tmp);
    free(tmp);
    words[(n + 2) * i + n + 1] = '\n';
  }
  words[n * number_words + 2 * number_words] = '\0';

  fwrite(words, sizeof(char), (n * number_words + 2 * number_words), stdout);
}
