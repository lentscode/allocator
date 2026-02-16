#define _ALLOCATOR_TEST
#include "allocator.h"

#include "stdio.h"
#include <string.h>


#define ASSERT(condition) \
	if (!(condition)) { fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, __func__); return; }

static inline void cleanup() {
	memset(chunk_list, 0, sizeof(struct chunk)*chunk_list_size);
}

void test__normal_allocation() {
	char *ptr = (char*)cust_malloc(1);
	ASSERT(ptr != NULL);

	*ptr = 'a';
	ASSERT(*ptr == 'a');

	cleanup();
}

void test__multiple_allocation() {
	void *start = NULL;
	void *end = NULL;
	for (size_t i = 10; i > 0; i--) {
		void *ptr = cust_malloc(i);
		if (i == 10) start = ptr;
		if (i == 1) end = ptr;
	}

	ASSERT(end - start == 55);
}

int main(void) {
	test__normal_allocation();
	return 0;
}
