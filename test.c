#include <stdlib.h>
#include <stdio.h>

// #define MC_MALLOC  malloc2
// #define MC_REALLOC realloc2

#include "madcrow_list.h"
madcrow_list_create(alist,SizeList,size_t);

#include "madcrow_buffer.h"
madcrow_buffer_create(abuf,SizeBuffer,size_t);

#include "madcrow_linkedlist.h"
madcrow_linkedlist_create(llist,LinkedList,LinkedNode,size_t);

static void test_list()
{
  size_t i;
  SizeList alist;
  alist_alloc(&alist, 1024);

  alist_push(&alist, 1);
  alist_push(&alist, 2);
  alist_push(&alist, 3);

  assert(alist_length(&alist) == 3);

  alist_pop(&alist);
  alist_pop(&alist);
  alist_pop(&alist);

  assert(alist_length(&alist) == 0);

  alist_shift(&alist, 1);
  alist_shift(&alist, 2);
  alist_shift(&alist, 3);

  assert(alist_length(&alist) == 3);

  alist_unshift(&alist);
  alist_unshift(&alist);
  alist_unshift(&alist);

  assert(alist_length(&alist) == 0);

  alist_push(&alist, 14);
  alist_push(&alist, 15);
  alist_push(&alist, 16);
  alist_shift(&alist, 13);
  alist_shift(&alist, 12);
  alist_shift(&alist, 11);

  assert(alist_length(&alist) == 6);

  for(i = 0; i < 6; i++) assert(*alist_get(&alist, i) == i+11);

  alist_dealloc(&alist);
}

static void test_buffer()
{
  size_t i;
  SizeBuffer abuf;
  abuf_alloc(&abuf, 8);

  for(i = 0; i < 100; i++) assert(i == abuf_add(&abuf, i));
  for(i = 0; i < 100; i++) assert(abuf.data[i] == i);

  abuf_dealloc(&abuf);
}

int main()
{
  #ifdef NDEBUG
    printf("Must compile without NDEBUG=1\n");
    exit(-1);
  #endif

  test_list();
  test_buffer();

  printf("  Tests Finished. Zero Errors\n");
  return 0;
}
