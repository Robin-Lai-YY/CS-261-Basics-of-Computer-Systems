#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#include "implicit.h"

/*
 * Determine whether or not a block is in use.
 */
static inline int block_is_in_use(void *block_start) {
  return 1 & *((block_size_t *) block_start);
}

/*
 * Return the size of a block.
 */
static inline block_size_t get_block_size(void *block_start) {
  return -HEADER_SIZE & *((block_size_t *) block_start);
}

/*
 * Return the size of the payload of a block.
 */
static inline block_size_t get_payload_size(void *block_start) {
  return get_block_size(block_start) - HEADER_SIZE * 2;
}

/*
 * Find the start of the block, given a pointer to the payload.
 */
static inline void *get_block_start(void *payload) {
  return payload - HEADER_SIZE;
}

/*
 * Find the payload, given a pointer to the start of the block.
 */
static inline void *get_payload(char *block_start) {
  return block_start + HEADER_SIZE;
}

/*
 * Set the size of a block, and whether or not it is in use. Remember each block
 * has two copies of the header (one at each end).
 */
static inline void set_block_header(void *block_start, block_size_t block_size, int in_use) {
  
  block_size_t header_value = block_size | in_use;
  *((block_size_t *) block_start) = header_value;
  *((block_size_t *) (get_payload(block_start) +
		      get_payload_size(block_start))) = header_value;
}

/*
 * Find the start of the next block.
 */
static inline void *get_next_block(void *block_start) {
  
  return block_start + get_block_size(block_start);
}


/*
 * Find the start of the previous block.
 */
static inline void *get_previous_block(void *block_start) {
  
  /* TO BE COMPLETED BY THE STUDENT. */
  return block_start - get_block_size(block_start - HEADER_SIZE);
}

/*
 * Determine whether or not the given block is at the front of the heap.
 */
static inline int is_first_block(heap *h, void *block_start) {
  
  return block_start == h->start;
}

/*
 * Determine whether or not the given address is inside the heap
 * region. Can be used to loop through all blocks:
 *
 * for (blk = h->start; is_within_heap_range(h, blk); blk = get_next_block(blk)) ...
 */
static inline int is_within_heap_range(heap *h, void *addr) { // 1 means in, 0 otherwise
  
  return addr >= h->start && addr < h->start + h->size;
}

/*
 * Coalesce a block with its consecutive block, but only if both
 * blocks are free. Returns a pointer to the beginning of the
 * coalesced block.
 */
static inline void *coalesce(heap *h, void *first_block_start) {
  if(!is_within_heap_range(h, first_block_start) || !is_within_heap_range(h, get_next_block(first_block_start))) 
  {
    return first_block_start;
  }
  else
  {
    if(block_is_in_use(first_block_start) || block_is_in_use(get_next_block(first_block_start))) 
    {
      return first_block_start;
    }
    else
    {
      set_block_header(first_block_start, get_block_size(first_block_start) + get_block_size(get_next_block(first_block_start)), 0);
      if(h->next == get_next_block(first_block_start) && h->search_alg == HEAP_NEXTFIT)
      {
        h->next = first_block_start;
      }
    }
  }
  return first_block_start; 
}

/*
 * Determine the size of the block we need to allocate given the size
 * the user requested. Don't forget we need space for the header and
 * footer, and that the user size may not be aligned.
 */
static inline block_size_t get_size_to_allocate(block_size_t user_size) {
  if(user_size % 8 == 0) return user_size + 2 * HEADER_SIZE;

  
}

/*
 * Turn a free block into one the user can utilize. Split the block if
 * it's more than twice as large or MAX_UNUSED_BYTES bytes larger than
 * needed.
 */
static inline void *prepare_block_for_use(void *block_start, block_size_t real_size) {
  if(get_size_to_allocate(real_size) + MAX_UNUSED_BYTES < get_block_size(block_start) || get_size_to_allocate(real_size) * 2 < get_block_size(block_start)){
    set_block_header(block_start + get_size_to_allocate(real_size),get_block_size(block_start) - get_size_to_allocate(real_size), 0);
    set_block_header(block_start, get_size_to_allocate(real_size), 1);
    return block_start + HEADER_SIZE;
  }
  set_block_header(block_start,get_block_size(block_start),1); 
  return block_start + HEADER_SIZE;
}

/*
 * Create a heap that is "size" bytes large, including its header.
 */
heap *heap_create(intptr_t size, search_alg_t search_alg) {
  
  /* Allocate space in the process' actual heap */
  void *heap_start = sbrk(size);
  if (heap_start == (void *) -1) return NULL;
  
  /* Use the first part of the allocated space for the heap header */
  heap *h     = heap_start;
  heap_start += sizeof(heap);
  size       -= sizeof(heap);
  
  /* Ensures the heap_start points to an address that has space for
     the header, while allowing the payload to be aligned to PAYLOAD_ALIGN */
  int delta = PAYLOAD_ALIGN - HEADER_SIZE - ((uintptr_t) heap_start) % PAYLOAD_ALIGN;
  if (delta < 0)
    delta += PAYLOAD_ALIGN;
  if (delta > 0) {
    heap_start += delta;
    size -= delta;
  }
  
  /* Ensures the size points to as many bytes as necessary so that
     only full-sized blocks fit into the heap. */
  size -= (size - 2 * HEADER_SIZE) % PAYLOAD_ALIGN;
  
  h->size = size;
  h->start = heap_start;
  h->search_alg = search_alg;
  h->next = h->start;
  
  set_block_header(h->start, size, 0);
  return h;
}
/*
 * Print the structure of the heap to the screen.
 */
void heap_print(heap *h) {
  void*current = h->start;
  while(is_within_heap_range(h, current)){
    printf("Block at address %p\n", current);
    printf(" Size: %d\n", get_block_size(current));
    if(block_is_in_use(current)){
      printf(" In use: Yes--------------------------------\n");
    }
    else{
      printf(" In use: No\n");
    }
    current = get_next_block(current);
  }

}

/*
 * Determine the average size of all free blocks.
 */
block_size_t heap_find_avg_free_block_size(heap *h) {
  
   int size = 0;
  int num_of_block = 0;
  void* current = h->start;
  while(is_within_heap_range(h, current)){
    if(!block_is_in_use(current)){
      size = size + get_block_size(current);
      num_of_block++;
    }
    current = get_next_block(current);
  } 

  //heap_print(h);
  printf("Heap size:  %ld\n", h->size);
  printf("Total size of free blocks:  %d\n", size);
  printf("Num of free blocks:  %d\n", num_of_block);

  /* TO BE COMPLETED BY THE STUDENT. */
  return size/num_of_block;
  return 0;
}

/*
 * Free a block on the heap h. Beware of the case where the heap uses
 * a next fit search strategy, and h->next is pointing to a block that
 * is to be coalesced.
 */
void heap_free(heap *h, void *payload) {
  void * start = get_block_start(payload);
  set_block_header(start, get_block_size(start), 0);
  coalesce(h, start);
  coalesce(h, get_previous_block(start));
  /* TO BE COMPLETED BY THE STUDENT. */
}

/*
 * Malloc a block on the heap h, using first fit. Return NULL if no
 * block large enough to satisfy the request exists.
 */
static void *malloc_first_fit(heap *h, block_size_t user_size) {
  
 void* current = h->start;
  while(is_within_heap_range(h, current)){
    if(!block_is_in_use(current)){
      if(get_block_size(current) >= get_size_to_allocate(user_size)){
        void * result = prepare_block_for_use(current, user_size);
        return result;
      }  
    }
    current = get_next_block(current);
  }
  
  return NULL;
}

/*
 * Malloc a block on the heap h, using best fit. Return NULL if no block
 * large enough to satisfy the request exists.
 */
static void *malloc_best_fit(heap *h, block_size_t user_size) {
  
   int smallest_difference = h -> size;

  void* current = h -> start;
  while(is_within_heap_range(h, current)){
    if(!block_is_in_use(current)){
      if(get_block_size(current) >= get_size_to_allocate(user_size)){
        int this_difference = get_block_size(current) - get_size_to_allocate(user_size);
        if(this_difference <= smallest_difference){
          smallest_difference = this_difference;
          best_block = current;
        }
      }  
    }
    current = get_next_block(current);
  }
  if(best_block != NULL){
    void * result = prepare_block_for_use(best_block, user_size);
    return result;
  }
  return NULL;
}

/*
 * Malloc a block on the heap h, using next fit. Return NULL if no block
 * large enough to satisfy the request exists.
 */
static void *malloc_next_fit(heap *h, block_size_t user_size) {
  
 void* current = h->next;
  while(is_within_heap_range(h, current)){
    if(!block_is_in_use(current)){
      if(get_block_size(current) >= get_size_to_allocate(user_size)){
        void * result = prepare_block_for_use(current, user_size);
        h->next = get_block_start(result);
        return result;
      }  
    }
    current = get_next_block(current);
  }
  current = h->start;
  while(is_within_heap_range(h, current)){
    if(!block_is_in_use(current)){
      if(get_block_size(current) >= get_size_to_allocate(user_size)){
        void * result = prepare_block_for_use(current, user_size);
        h->next = get_block_start(result);
        return result;
      }  
    }
    current = get_next_block(current);
  }


  return NULL;
}

/*
 * Our implementation of malloc.
 */
void *heap_malloc(heap *h, block_size_t size) {
  
  switch (h->search_alg) {
  case HEAP_FIRSTFIT:
    return malloc_first_fit(h, size);
  case HEAP_NEXTFIT:
    return malloc_next_fit(h, size);
  case HEAP_BESTFIT:
    return malloc_best_fit(h, size);
  }
  return NULL;
}
