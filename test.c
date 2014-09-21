#include <stdlib.h>
#include <stdio.h>

// #define BC_MALLOC  malloc2
// #define BC_REALLOC realloc2

#include "obj_macro_list.h"
obj_macro_list_create(alist,SizeList,size_t)

#include "obj_macro_buffer.h"
obj_macro_buffer_create(abuf,SizeBuffer,size_t)

int main()
{
  SizeList alist;
  alist_alloc(&alist, 1024);



  alist_dealloc(&alist);
}
