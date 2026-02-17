#define _ALLOCATOR_TEST
#include "allocator.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#define ASSERT(condition) \
	if (!(condition)) {\
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, __func__);\
		cleanup();\
		return;\
	}

static inline void cleanup() {
	munmap(memory, memory_cap);
	munmap(chunk_list, chunk_list_cap);
	memory = NULL;
	chunk_list = NULL;
}

void test__normal_allocation() {
	ASSERT(memory == NULL);
	ASSERT(chunk_list == NULL);

	char *ptr = (char*)cust_malloc(1);
	ASSERT(memory != NULL);
	ASSERT(chunk_list != NULL);
	ASSERT(ptr != NULL);

	*ptr = 'a';
	ASSERT(*ptr == 'a');

	cleanup();
	ASSERT(memory == NULL);
	ASSERT(chunk_list == NULL);
}

void test__multiple_allocation() {
	void *start = NULL;
	void *end = NULL;
	for (size_t i = 10; i > 0; i--) {
		void *ptr = cust_malloc(i);
		if (i == 10) start = ptr;
		if (i == 1) end = ptr;
	}

	ASSERT(end - start == 55 - 1);

	cust_free(start);
	ASSERT(chunk_list[0].size == 9);

	cust_malloc(8);
	ASSERT(chunk_list[0].ptr == start);
	ASSERT(chunk_list[0].size == 8);
	ASSERT(chunk_list[1].ptr == start + 10);
	ASSERT(chunk_list[1].size == 9);

	cleanup();
}

int main(void) {
	test__normal_allocation();
	test__multiple_allocation();
	return 0;
}
