#include <stdlib.h>
#include <stdio.h>

// #define MC_MALLOC  malloc2
// #define MC_REALLOC realloc2

#include "madcrow_list.h"
madcrow_list_create(alist,SizeList,size_t)

#include "madcrow_buffer.h"
madcrow_buffer_create(abuf,SizeBuffer,size_t)

int main()
{
  SizeList alist;
  alist_alloc(&alist, 1024);



  alist_dealloc(&alist);
}
