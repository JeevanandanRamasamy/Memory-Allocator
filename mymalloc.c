#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

typedef struct block {
    struct block *prev;
    struct block *next;
    size_t size;
    int status;
    void* address;
} block;

void* heap;
block* nextFitPtr;
int alg;

void myinit(int allocAlg) {
    alg = allocAlg;
    heap = malloc(1 << 20);
    block head;
    head.prev = NULL;
    head.next = NULL;
    head.size = (1 << 20) - sizeof(block);
    head.status = 0;
    head.address = heap + sizeof(block);
    memmove(heap, &head, sizeof(block));
}

void* allocate(block* n, int s) {
    if (n->size > s + sizeof(block)) {
        block* split = (void*) n + s + sizeof(block);
        split->prev = n;
        split->next = n->next;
        split->size = n->size - s - sizeof(block);
        n->size = s;
        split->address = (void*) split + sizeof(block);
        split->status = 0;
        memmove(n + sizeof(block) + s, split, sizeof(block));
        if (n->next)
            n->next->prev = split;
        n->next = split;
        nextFitPtr = split;
        n->status = 1;
        return n->address;
    }
    else if (n->size >= s) {
        n->status = 1;
        nextFitPtr = n->next;
        return n->address;
    }
    else
        return NULL;
}

void* mymalloc(size_t size) {
    if (size == 0)
        return NULL;
    if (size % 8 != 0)
        size += 8 - size % 8;
    if (alg == 2) {
        block* min = NULL;
        for (block* curr = (block*) heap; curr; curr = curr->next) {
            if ((!min || (min && curr->size < min->size)) && curr->status == 0 && curr->size >= size)
                min = curr;
        }
        if (min)
            return allocate(min, size);
    }
    else {
        block* start;
        if (alg == 1 && nextFitPtr)
            start = nextFitPtr;
        else
            start = (block*) heap;
        for (block* curr = start; curr; curr = curr->next) {
            if (curr->status == 0 && curr->size >= size) {
                block* n = allocate(curr, size);
                if (n)
                    return n;
            }
        }
        if (alg == 1) {
            for (block* curr = (block*) heap; curr != nextFitPtr; curr = curr->next) {
                if (curr->status == 0 && curr->size >= size) {
                    block* n = allocate(curr, size);
                    if (n)
                        return n;
                }
            }
        }
    }
    return NULL;
}

void coalesceNext(block* n) {
    n->size += n->next->size + sizeof(block);
    n->next = n->next->next;
    if (n->next)
        n->next->prev = n;
}

void myfree(void* ptr) {
    for (block* curr = (block*) heap; curr; curr = curr->next) {
        if (curr->address == ptr && curr->status == 1) {
            curr->status = 0;
            if (curr->next && curr->next->status == 0)
                coalesceNext(curr);
            if (curr->prev && curr->prev->status == 0)
                coalesceNext(curr->prev);
            return;
        }
    }
}

void* myrealloc(void* ptr, size_t size) {
    if (ptr && size) {
        block* n;
        for (block* curr = (block*) heap; curr; curr = curr->next) {
            if (curr->address == ptr && curr->status == 1) {
                n = curr;
                break;
            }
        }
        void* newptr;
        if (size % 8 != 0)
            size += 8 - size % 8;
        if (n->size >= size)
            newptr = allocate(n, size);
        else if (n->next && n->next->status == 0 && n->size + n->next->size + sizeof(block) >= size) {
            coalesceNext(n);
            newptr = allocate(n, size);
        }
        else if (n->prev && n->prev->status == 0 && n->size + n->prev->size + sizeof(block) >= size) {
            coalesceNext(n->prev);
            newptr = allocate(n->prev, size);
            memmove(newptr, ptr, size);
        }
        else if (n->prev && n->next && n->prev->status == 0 && n->next->status == 0 && 
                 n->size + n->prev->size + n->next->size + 2 * sizeof(block) >= size) {
            coalesceNext(n);
            coalesceNext(n->prev);
            newptr = allocate(n->prev, size);
            memmove(newptr, ptr, size);
        }
        else {
            newptr = mymalloc(size);
            memmove(newptr, ptr, size);
            myfree(ptr);
        }
        return newptr;
    }
    else if (ptr) {
        myfree(ptr);
        return NULL;
    }
    else if (size)
        return mymalloc(size);
    return NULL;
}

void mycleanup() {
    free(heap);
    nextFitPtr = NULL;
}