
all: test

run_tests: test.c madcrow_list.h madcrow_buffer.h madcrow_linkedlist.h
	$(CC) -std=c99 -Wall -Wextra -o $@ $<

test: run_tests
	./run_tests

clean:
	rm -rf run_tests

.PHONY: all clean test
