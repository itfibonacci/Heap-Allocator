# Memory Allocator in C

[Writing a memory allocator](https://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/#video-lecture)

[Snippet for a memory allocator in C](https://gist.github.com/apsun/caa3c5552dce7b13b898b70569b1f239)

[Memory Allocators 101 - Write a simple memory allocator](https://arjunsreedharan.org/post/148675821737/memory-allocators-101-write-a-simple-memory)

## Things to consider
- Memory Alignment - padding

## Functions to implement highest level
```c
void *malloc(size_t size);
void free(void *ptr);
// the above two will be implemented first later calloc and realloc
```

## Steps to implement
- void initialize_allocator(size_t size)
	- this function will be called the first time when a memory allocation request is sent. it will create a large block with a certain size.

## High level details from chatgpt

1. **Define the Data Structure**: You'll need a data structure to keep track of the size and allocation status of each block. This could be a simple struct with size and a flag indicating whether the block is free or not.

2. **Initialize the Heap**: When your program starts, you'll need to set aside a chunk of memory to act as your heap. This can be done using the `mmap()` system call.

3. **Implement `malloc()`:** This function should find a free block that's large enough to satisfy the request. If no such block exists, it should increase the size of the heap. Once a suitable block is found, it should be marked as allocated and its address returned.

4. **Implement `free()`:** This function should take a pointer to a block of memory, mark it as free, and potentially reduce the size of the heap if the freed block is at the end.

5. **Implement `realloc()`:** This function should resize an allocated block. This might involve finding a new block and copying the old data over.

6. **Handle Fragmentation**: Over time, the heap can become fragmented with free blocks scattered throughout. You might want to implement a strategy for defragmenting the heap, such as coalescing adjacent free blocks.

## Why Blocks?

In the context of a heap allocator, "blocks" refer to chunks of memory within the heap. Here's why you need them and how they work:

1. **Why Blocks?**: When you're managing memory, it's helpful to divide it into manageable chunks, or "blocks". Each block can be allocated and deallocated independently. This allows for efficient use of memory, as you can allocate exactly as much memory as you need, no more and no less.

2. **Block Structure**: Each block typically contains metadata about the block (such as its size and whether it's free or allocated) and the actual data stored in the block. The metadata is used by the allocator to manage the memory.

3. **Allocation**: When you call `malloc()`, the allocator looks for a free block that's large enough to hold the requested amount of data. If it finds one, it marks that block as allocated and returns a pointer to the data portion of the block.

4. **Deallocation**: When you call `free()`, the allocator marks the block as free, making it available for future allocations. If the freed block is at the end of the heap, the allocator might also reduce the size of the heap to free up memory.

5. **Fragmentation**: Over time, as blocks are allocated and deallocated, the heap can become fragmented. This means that the free memory is divided into small, non-contiguous blocks. This can make it hard to find a large enough block for a `malloc()` call, even if there's enough total free memory. To handle this, allocators often include logic to defragment the heap, such as coalescing adjacent free blocks.

## Todo

1. Add actual memory freeing from the process space, i.e. munmap
   1. Currently there is an issue with the my_free function. The test for it is returning a seg fault.
2. Understand and improve the data alignment / padding part. [Data Structure Alignment]([https://](https://en.wikipedia.org/wiki/Data_structure_alignment#:~:text=Data%20structure%20padding,-Although%20the%20compiler&text=To%20maintain%20proper%20alignment%20the,structures%20to%20be%20properly%20aligned.))
3. is our code correct in the sense of blocks not somehow being created where a data pointed by the block should be. 
4. Comprehensive testing needed for the malloc
