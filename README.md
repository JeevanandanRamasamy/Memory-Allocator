# Memory-Allocator

This program is a custom implementation of malloc, free, and realloc. To use these methods, it is best to use a separate c file with the main method and linking the files together.

## void myinit(int allocAlg)

Creates a 1 MB “heap” and other initializations the code needs. Any application using this library must call this function first. The allocAlg argument describes what algorithm to use to find a free block:

0: first fit

1: next fit

2: best fit

## void* mymalloc(size_t size)

From the “heap”, allocates a region of at least the requested size and returns a pointer to the beginning of the region. If it cannot be allocated, returns NULL.

All returned addresses must be 8-byte aligned. That is, the region allocated starts at an address that’s divisible by 8.

If size is 0, mymalloc does nothing and returns NULL.

## void myfree(void* ptr)

Marks the given region as free and available to be allocated for future requests. It is coalesced with adjacent free regions.

This implementation uses an implicit free list.

If ptr is NULL, myfree does nothing.

## void* myrealloc(void* ptr, size_t size)

Reallocate the region pointed to by ptr to be at least the new given size. If this cannot be done in-place, a new region is allocated, the data from the original region is copied over, and the old region is freed.

If the reallocation can’t be done, returns NULL.

If ptr is NULL, this is equivalent to mymalloc(size).

If size is 0, this is equivalent to myfree(ptr) and myrealloc returns NULL.

If both ptr is NULL and size is 0, myrealloc does nothing and returns NULL.

## void mycleanup()

Frees the 1 MB “heap” and performs other cleanup the code needs. Any application using this library must call this function last.

The library supports “resetting” everything by calling mycleanup followed by myinit.
