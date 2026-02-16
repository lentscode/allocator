#include "allocator.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>

#define CHUNK_LIST_SIZE (sizeof(struct chunk) * 1024)
#define MIN_MAP_SIZE ((size_t)4096)

struct chunk {
  void *ptr;
  size_t size;
};

struct chunk *chunk_list = NULL;
size_t chunk_list_size = 0;

void *memory = NULL;
size_t memory_cap = 0;

static int _init() {
  memory = (char *)mmap(NULL, MIN_MAP_SIZE, PROT_WRITE | PROT_READ,
                        MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (memory == MAP_FAILED)
    return -1;
  memory_cap = MIN_MAP_SIZE;

  chunk_list =
      (struct chunk *)mmap(NULL, CHUNK_LIST_SIZE, PROT_WRITE | PROT_READ,
                           MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (chunk_list == MAP_FAILED)
    return -1;

  return 0;
}

static void _chunk_list_dump() {
  printf("Chunk dump, size = %lu\n", chunk_list_size);
  for (size_t i = 0; i < chunk_list_size; i++) {
    printf("%p -> %lu\n", chunk_list[i].ptr, chunk_list[i].size);
  }
}

static int _find_chunk(void *ptr) {
  size_t low = 0;
  size_t high = chunk_list_size - 1;

  for (; low <= high;) {
    size_t mid = (low + high) / 2;

    if (ptr == chunk_list[mid].ptr)
      return mid;

    if (ptr < chunk_list[mid].ptr)
      high = mid - 1;
    else
      low = mid + 1;
  }

  return -1;
}

void *cust_malloc(size_t size) {
  if (size == 0)
    return NULL;
  if (size > memory_cap)
    return NULL;

  if (chunk_list_size == 0) {
    chunk_list[0].ptr = memory;
    chunk_list[0].size = size;
    chunk_list_size++;
    return memory;
  }

  void *new_ptr = memory;
  size_t chunk_size = 0;
  size_t i = 0;
  for (; i < chunk_list_size; i++) {
    struct chunk curr = chunk_list[i];

    if (curr.ptr - (new_ptr + chunk_size) >= size)
      break;

    new_ptr = curr.ptr;
    chunk_size = curr.size;
  }

  for (size_t j = chunk_list_size; j > i; j--)
    chunk_list[j] = chunk_list[j - 1];
  chunk_list_size++;

  if (i > 0) {
    new_ptr = chunk_list[i - 1].ptr + chunk_list[i - 1].size;
  } else {
    new_ptr = memory;
  }
  chunk_list[i].size = size;
  chunk_list[i].ptr = new_ptr;

  return new_ptr;
}

void cust_free(void *ptr) {
  if (ptr == NULL || memory == NULL)
    return;
  if (ptr < memory || ptr >= memory + memory_cap)
    return;
  if (chunk_list_size == 0)
    return;

  int index = _find_chunk(ptr);
  if (index < 0)
    return;
  printf("%d\n", index);

  for (size_t i = index; i < chunk_list_size - 1; i++) {
    chunk_list[i].size = chunk_list[i + 1].size;
    chunk_list[i].ptr = chunk_list[i + 1].ptr;
  }
  chunk_list_size--;
}
