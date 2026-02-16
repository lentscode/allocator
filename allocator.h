#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stddef.h>

void *cust_malloc(size_t size);
void cust_free(void *ptr);

#ifdef _ALLOCATOR_TESTING
extern struct chunk *chunk_list;
extern size_t chunk_list_size;
#endif

#endif // _ALLOCATOR_H
