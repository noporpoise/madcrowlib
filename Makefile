
all: test

test: test.c obj_macro_list.h obj_macro_buffer.h
	$(CC) -std=c99 -Wall -Wextra -pedantic -o $@ $<

clean:
	rm -rf test

.PHONY: all
