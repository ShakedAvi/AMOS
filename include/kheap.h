#ifndef KHEAP_H
#define KHEAP_H

#define KHEAP_START         0x40000000
#define KHEAP_INITIAL_SIZE  0x1000000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0xDEADBEEF
#define HEAP_MIN_SIZE     0x70000

#include "types.h"
#include "orderedArray.h"
#include "paging.h"

/**
   Size information for a hole/block
**/
typedef struct header_t
{
  uint32 magic;   // Magic number, used for error checking and identification.
  uint8 is_hole;   // 1 if this is a hole. 0 if this is a block.
  uint32 size;    // size of the block, including the end footer.
}header_t;

typedef struct footer_t
{
  uint32 magic;     // Magic number, same as in header_t.
  header_t *header; // Pointer to the block header.
}footer_t;

typedef struct heap_t
{
  ordered_array_t index;
  uint32 start_address; // The start of our allocated space.
  uint32 end_address;   // The end of our allocated space. May be expanded up to max_address.
  uint32 max_address;   // The maximum address the heap can be expanded to.
  uint8 supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
  uint8 readonly;       // Should extra pages requested by us be mapped as read-only?
}heap_t;

/*
  The function creates a new heap.

  Input:
    The heap's data.
  Output:
    The final heap.
*/
heap_t* create_heap(uint32 start, uint32 end, uint32 max, uint8 supervisor, uint8 readonly);

/*
  The function allocates a contiguous region of memory 'size' in size. If page_align==1, it creates that block starting
  on a page boundary.

  Input:
    The size to allocate and the parameters mentioned above.
  Output:
    None.
*/
void* alloc(uint32 size, uint8 page_align, heap_t* heap);

/*
  The function releases a block allocated with 'alloc'.

  Input:
    A pointer to the block to free and to the heap to free the block from.
  Output:
    None.
*/
void free(void* p, heap_t* heap);

/*
  The function allocates an amount of memory. If align == 1,
  the chunk must be page-aligned. If phys != 0, the physical
  location of the allocated chunk will be stored into phys.

  This is the internal version of kmalloc. More user-friendly
  parameter representations are available in kmalloc, kmalloc_a,
  kmalloc_ap, kmalloc_p.

  Input:
    The size to allocate and the parameters mentioned above.
  Output:
    A pointer to the allocated memory.
*/
uint32 kmalloc_int(uint32 sz, int align, uint32* phys);

/*
  The function allocates a pointer.

  Input:
    The size to allocate.
  Output:
    A pointer to the allocated memory.
*/
uint32 kmalloc_a(uint32 sz);

/*
  The function allocates an amount of memory, and returns the physical address
  which were allocated too.

  Input:
    The size to allocate and the start of the physical address.
  Output:
    A pointer to the allocater memory.
*/
uint32 kmalloc_p(uint32 sz, uint32* phys);

/*
  The function allocates an amount of memory, and returns the physical address
  which were allocated too.

  Input:
    The size to allocate and the start of the physical address.
  Output:
    A pointer to the allocater memory.
*/
uint32 kmalloc_ap(uint32 sz, uint32* phys);

/*
  The function allocates a pointer.

  Input:
    The size to allocate.
  Output:
    A pointer to the allocated memory.
*/
uint32 kmalloc(uint32 sz);

/*
  The function deallocates a pointer.

  Input:
    The pointer to free.
  Output:
    None.
*/
void kfree(void* p);

/*
  The function returns the amount of space the input pointer has alloced.

  Input:
    The allocated pointer to check.
  Output:
    The amount of allocated space.
*/
uint32 size_of_alloc(void* alloc);

/*For reallocation of kmallocs, regular, a, p, ap*/
uint32 krealloc(uint32* ptr, uint32 old_sz, uint32 new_sz);
uint32 krealloc_a(uint32* ptr, uint32 old_sz, uint32 new_sz);
uint32 krealloc_ap(uint32* ptr, uint32 old_sz, uint32 new_sz, uint32* phys);
uint32 krealloc_p(uint32* ptr, uint32 old_sz, uint32 new_sz, uint32* phys);

/*
  The function expands a given heap.

  Input:
    The heap to expand and the new size for it.
  Output:
    None.
*/
void expand(uint32 new_size, heap_t* heap);

#endif
