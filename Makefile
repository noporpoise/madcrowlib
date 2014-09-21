
all: test

test: test.c madcrow_list.h madcrow_buffer.h
	$(CC) -std=c99 -Wall -Wextra -pedantic -o $@ $<

clean:
	rm -rf test

.PHONY: all
