#include <errno.h>

#include "free.h"
/* 
return 0 if success and non-zero otherwise
- what about actually releasing memory from the process not just marking the block free***
- at some point can add checks to make sure the passed pointer does point to a block
- make sure to coalesce free blocks next to each other - to reduce fragmentation
- returns 1 if block is already free, 2 if the block got freed but munmap failed.
- returns 3 if block pointed to by ptr is NULL or if ptr passed is null
*/
unsigned char my_free(void *ptr) {
	// do i need a block finder function here
	Block *block = (Block*)ptr - 1;

	if (ptr == NULL || block == NULL) {
		return 3;
	}

	if (block->free == 1) {
		return 1;
	}

	// merge blocks
	block = merge_blocks(block);
	
	if (block->free == 0)
	{
		// Mark the block as free
		block->free = 1;
		
		// check if the block is at the end of the heap and reduce the heap size
		if (block->next == NULL) {
			// if the current block is not null and it has a prev. we change the previous block's next to be NULL
			if (block != NULL && block->prev != NULL) {
				block->prev->next = NULL;
			}

			if (munmap(block, block->size) != 0) {
				return errno;
			};
		}
	}
	return 0;
}

Block *merge_blocks(Block* block) {
	if (block == NULL) {
		return NULL;
	}
	
	/* thinking about doing a while loop, at first to the "left" of the passed block.
	we go to the previous block check if it is free, if it is we merge them together
	when we merge we have to make sure to remove the info about the merging block */

	// let's keep track of the size of the blocks counted so far
	size_t new_size = 0;	
	Block *last_visited_block = block;
	Block *current_block = block;
	
	// first let's go back until we see a non-free block
	while(current_block != NULL && current_block->free == 1) {
		new_size += current_block->size;
		last_visited_block = current_block;
		current_block = current_block->prev;
	}
	/* when we come out of the while loop current_block will either be NULL or not free. in the case when current_block is NULL means that the next block of current block should become the new beginning block. */

	// first block to start combining from
	Block *beginning_block = last_visited_block;
	
	// let's go back to the block's next one since block has already been visited and start going to the right and finding free blocks next to each other to combine
	current_block = block->next;
	last_visited_block = block->next;
	while(current_block != NULL && current_block->free == 1) {
		new_size += current_block->size;
		last_visited_block = current_block;
		current_block = current_block->next;
	}
	// this last_block is the last one that we can combine.
	Block *last_block = last_visited_block;

	Block *combined_free_block = beginning_block;
	combined_free_block->free = 1;
	combined_free_block->prev = beginning_block->prev;
	combined_free_block->next = last_block->next;
	combined_free_block->size = new_size;

	// update the blocks before combined_block and after it.
	if (combined_free_block->prev != NULL) {
		combined_free_block->prev->next = combined_free_block;
	}
	if (combined_free_block->next != NULL) {
		combined_free_block->next->prev = combined_free_block;
	}

	return combined_free_block;
}
