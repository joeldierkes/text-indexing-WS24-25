CC = gcc
# CFLAGS  = -g -Wall -flto
CFLAGS  = -g -O2 -flto

default: ti_programm

ti_programm: ti_programm.c trie.h trie.c specific_trie.h fixed_arr.c priority_queue_linked_list.o variable_list.c hashmap_specific.c
	@echo $<
	$(CC) $(CFLAGS) -o $@ ti_programm.c trie.c fixed_arr.c priority_queue_linked_list.o variable_list.c hashmap_specific.c

priority_queue_linked_list.o: priority_queue.h priority_queue_linked_list.c
	$(CC) $(CFLAGS) -c priority_queue_linked_list.c

.PHONY: clean
clean:
	$(RM) ti_programm *.o *~

.PHONY: test
test: ti_programm
	$(MAKE) -C tests

.PHONY: init-submodules
init-submodules:
	git submodule update --init --recursive

.PHONY: sqlplot-tools
sqlplot-tools: init-submodules
	@( \
		cd sqlplot-tools && \
		mkdir -p build && \
		cd build && \
		cmake -DWITH_POSTGRESQL=OFF -DWITH_MYSQL=OFF .. \
	)
	$(MAKE) -C sqlplot-tools/build
