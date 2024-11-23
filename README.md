# Memory-Allocator

**Memory-Allocator** is a custom implementation of memory management functions, including `malloc`, `free`, and `realloc`. This library provides an efficient memory allocation system, allowing developers to manage dynamic memory manually in C programs. To use these functions, it's best to create a separate C file with the `main` function and link it with the provided source files.

---

## Functions

### `void myinit(int allocAlg)`

Initializes the memory allocator with a 1 MB "heap" and sets up the required data structures. This function must be called first in any application using this library. 

- **`allocAlg`**: Specifies the algorithm to be used for finding a free block:
  - `0`: **First Fit** – Allocates the first available block that fits the size.
  - `1`: **Next Fit** – Allocates the next available block that fits the size, starting from the last allocated block.
  - `2`: **Best Fit** – Allocates the smallest available block that fits the requested size.

---

### `void* mymalloc(size_t size)`

Allocates a region of memory of at least the requested size from the "heap" and returns a pointer to the beginning of the region. The memory is 8-byte aligned.

- If the allocation fails, it returns `NULL`.
- If the requested size is `0`, `mymalloc` does nothing and returns `NULL`.

---

### `void myfree(void* ptr)`

Marks the given memory region as free and available for future allocations. Adjacent free regions are coalesced to form larger blocks.

- The implementation uses an **implicit free list** for memory management.
- If the pointer is `NULL`, `myfree` does nothing.

---

### `void* myrealloc(void* ptr, size_t size)`

Reallocates the memory region pointed to by `ptr` to at least the new requested size. If the reallocation cannot be done in-place, it allocates a new region, copies the data from the original region, and frees the old memory block.

- If reallocation is not possible, it returns `NULL`.
- If `ptr` is `NULL`, it behaves like `mymalloc(size)`.
- If `size` is `0`, it behaves like `myfree(ptr)` and returns `NULL`.
- If both `ptr` is `NULL` and `size` is `0`, `myrealloc` does nothing and returns `NULL`.

---

### `void mycleanup()`

Frees the 1 MB "heap" and performs any necessary cleanup. This function must be called last in any application using this library.

- Calling `mycleanup` allows the system to reset by releasing all allocated memory and freeing up resources.
- If needed, the allocator can be reset by calling `mycleanup` followed by `myinit`.

---

## Usage

1. **Create a separate C file** that includes the `main` function and links with this library.
2. **Call `myinit`** at the start of your program to initialize the memory allocator.
3. **Use `mymalloc`, `myfree`, and `myrealloc`** to manage memory throughout your program.
4. **Call `mycleanup`** at the end of your program to free up resources and reset the allocator.

---

## Example

```c
#include "memory_allocator.h"

int main() {
    myinit(0);  // Initialize the memory allocator with the "First Fit" algorithm

    // Allocate memory for an integer array of 10 elements
    int* arr = (int*)mymalloc(10 * sizeof(int));

    if (arr == NULL) {
        // Handle memory allocation failure
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Reallocate memory to increase the size of the array
    arr = (int*)myrealloc(arr, 20 * sizeof(int));

    if (arr == NULL) {
        // Handle memory reallocation failure
        printf("Memory reallocation failed!\n");
        return 1;
    }

    // Free the allocated memory
    myfree(arr);

    // Clean up at the end of the program
    mycleanup();
    
    return 0;
}
```
