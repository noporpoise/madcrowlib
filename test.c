#include <stdlib.h>
#include <stdio.h>

// #define MC_CALLOC  calloc2
// #define MC_REALLOC realloc2

#include "madcrow_list.h"
madcrow_list(list,SizeList,size_t);

#include "madcrow_buffer.h"
madcrow_buffer(buf,SizeBuffer,size_t);
madcrow_buffer_wipe(zbuf,ZeroSizeBuffer,size_t);

#include "madcrow_linkedlist.h"
madcrow_linkedlist(llist,LinkedList,LinkedNode,size_t);

static void test_buffer()
{
  size_t i;
  SizeBuffer abuf;
  buf_alloc(&abuf, 8);

  for(i = 0; i < 100; i++) assert(i == buf_add(&abuf, i));
  for(i = 0; i < 100; i++) assert(abuf.data[i] == i);

  buf_dealloc(&abuf);
}

static void test_list()
{
  size_t i;
  SizeList alist;
  list_alloc(&alist, 8);

  list_push(&alist, 1);
  list_push(&alist, 2);
  list_push(&alist, 3);

  assert(list_length(&alist) == 3);

  list_pop(&alist);
  list_pop(&alist);
  list_pop(&alist);

  assert(list_length(&alist) == 0);

  list_shift(&alist, 1);
  list_shift(&alist, 2);
  list_shift(&alist, 3);

  assert(list_length(&alist) == 3);

  list_unshift(&alist);
  list_unshift(&alist);
  list_unshift(&alist);

  assert(list_length(&alist) == 0);

  list_push(&alist, 14);
  list_push(&alist, 15);
  list_push(&alist, 16);
  list_shift(&alist, 13);
  list_shift(&alist, 12);
  list_shift(&alist, 11);

  assert(list_length(&alist) == 6);

  for(i = 0; i < 6; i++) assert(*list_get(&alist, i) == i+11);

  list_dealloc(&alist);
}

static void test_linked_list()
{
  size_t i;
  LinkedList llist;
  llist_init(&llist);
  LinkedNode nodes[100];

  for(i = 0; i < 100; i++) {
    nodes[i].data = i;
    llist_push(&llist, &nodes[i]);
    assert(llist.last == &nodes[i]);
  }
}

int main()
{
  #ifdef NDEBUG
    printf("Must compile without NDEBUG=1\n");
    exit(-1);
  #endif

  test_buffer();
  test_list();
  test_linked_list();

  printf("  Tests Finished. Zero Errors\n");
  return 0;
}
