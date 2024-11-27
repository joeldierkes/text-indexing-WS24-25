//usr/bin/env tcc -run "$0" "$@" ; exit $?

// author: Joel Dierkes <dierkesjoel@gmail.com>
//
// A simple tool that creates fake data for the test-indexing
// WS2024/25 KIT module.
//
// Either run `./create_fake_data.c` after making the script
// executable and having the tiny C compiler (tcc) installed or
// compile it and run the generated executable. The output will be
// printed to stdout and can be redirected to a file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ALPHABET_SIZE (26 + 26 + 10)

// Branch free implementation that converts an alphabet index to the
// representative ASCII/UTF-8 character.
char index_to_char(size_t idx) {
  return (idx < 26) * (idx + 'a') +
         (26 <= idx && idx < 26 + 26) * (idx - 26 + 'A') +
         (26 + 26 <= idx && idx >= 26 + 26) * (idx - 26 - 26 + '0');
}

char *generate_random_word(unsigned int n) {
  char *ret = malloc((n + 1) * sizeof(char));

  size_t i = 0;
  for (; i < n; ++i) {
    ret[i] = index_to_char(rand() % ALPHABET_SIZE);
  }
  ret[i] = '\0';

  return ret;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: ./create_fake_data.c WORD_LEN NUMBER_WORDS\n"
	   "\n"
	   "Example: ./create_fake_data.c 8 1000 > example_data.txt\n");
    exit(1);
  }

  // It's the users task to provide valid integers.
  int n = atoi(argv[1]);
  int number_words = atoi(argv[2]);

  char *words =
      malloc((n * number_words + 2 * number_words + 1) * sizeof(char));
  for (int i = 0; i < number_words; ++i) {
    char *tmp = generate_random_word(n);
    strcpy(words + (n + 2) * i, tmp);
    free(tmp);
    words[(n + 2) * i + n + 1] = '\n';
  }
  words[n * number_words + 2 * number_words] = '\0';

  fwrite(words, sizeof(char), (n * number_words + 2 * number_words), stdout);
}
