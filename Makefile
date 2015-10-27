
ifdef DEBUG
	OPT=-O0 -ggdb -g
else
	OPT=-O3
endif

all: run_tests

run_tests: test.c madcrow_list.h madcrow_buffer.h madcrow_linkedlist.h
	$(CC) -std=c99 -Wall -Wextra $(OPT) -o $@ $<

test: run_tests
	./run_tests

clean:
	rm -rf run_tests

.PHONY: all clean test
