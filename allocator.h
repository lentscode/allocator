#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stddef.h>

#define CHUNK_LIST_SIZE (sizeof(struct chunk) * 1024)
#define MIN_MAP_SIZE ((size_t)4096)

void *cust_malloc(size_t size);
void cust_free(void *ptr);

extern struct chunk *chunk_list;
extern size_t chunk_list_size;
struct chunk {
	void *ptr;
	size_t size;
};

extern void *memory;
extern size_t memory_cap;

#endif // _ALLOCATOR_H
