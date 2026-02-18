# Simple Linear Allocator

`linall` (LINear ALLocator) is a simple custom linear allocator built for nothing in particular, just for personal learning and fun. It is not ready at all for production, but if you get tired of using `malloc` for some reason, you can try to include `linalloc.h` and `linalloc.c` in your project and see if it works.

It provides a simple interface to allocate and free memory, thanks to `void *linall_alloc(size_t size)` and `void linall_free(void *ptr)`. `linall` uses `chunks` for managing allocated memory, which are structs that contain a pointer to the reserved memory and the size in bytes of it.

The `mmap` POSIX call is used to reserve a memory region for the allocator. The size of it can be increased via `MIN_MAP_SIZE`.
