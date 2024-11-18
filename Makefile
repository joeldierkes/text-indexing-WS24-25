CC = gcc
CFLAGS  = -g -Wall -flto
# CFLAGS  = -g -O2 -flto

default: ti_programm

ti_programm: ti_programm.c trie.h trie.o fixed_arr.o
	@echo $<
	$(CC) $(CFLAGS) -o $@ ti_programm.c trie.o fixed_arr.o

trie.o: trie.h trie.c specific_trie.h
	$(CC) $(CFLAGS) -c trie.c

fixed_arr.o: specific_trie.h fixed_arr.c
	$(CC) $(CFLAGS) -c fixed_arr.c

.PHONY: clean
clean:
	$(RM) ti_programm *.o *~

.PHONY: test
test: ti_programm
	$(MAKE) -C tests

.PHONY: sqlplot-tools
sqlplot-tools:
	@( \
		cd sqlplot-tools && \
		mkdir -p build && \
		cd build && \
		cmake -DWITH_POSTGRESQL=OFF -DWITH_MYSQL=OFF .. \
	)
	$(MAKE) -C sqlplot-tools/build
