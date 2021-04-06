#include "kheap.h"

extern uint32 end;
uint32 placement_address = (uint32)&end;
extern page_directory_t* kernel_directory;
heap_t* kheap = 0;

//Physical memory used
volatile uint32 phys_mem_usage;

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
uint32 kmalloc_int(uint32 sz, int align, uint32* phys)
{
  if(kheap)
  {
    void* addr = alloc(sz, (uint8)align, kheap);
    if(phys)
    {
      page_t* page = get_page((uint32)addr, 0, kernel_directory);
      *phys = page->frame * 0x1000 + ((uint32)addr & 0xFFF);
    }

    phys_mem_usage += sz;
    return (uint32)addr;
  }
  else
  {
    if (align == 1 && (placement_address & 0xFFFFF000) )
    {
      placement_address &= 0xFFFFF000;
      placement_address += 0x1000;
    }
    if (phys)
    {
      *phys = placement_address;
    }
    uint32 tmp = placement_address;
    placement_address += sz;
    phys_mem_usage += sz;

    return tmp;
  }
}

/*
  The function deallocates a pointer.

  Input:
    The pointer to free.
  Output:
    None.
*/
void kfree(void* p)
{
  free(p, kheap);
}

/*
  The function allocates a pointer.

  Input:
    The size to allocate.
  Output:
    A pointer to the allocated memory.
*/
uint32 kmalloc_a(uint32 sz)
{
  return kmalloc_int(sz, 1, 0);
}

/*
  The function allocates an amount of memory, and returns the physical address
  which were allocated too.

  Input:
    The size to allocate and the start of the physical address.
  Output:
    A pointer to the allocater memory.
*/
uint32 kmalloc_p(uint32 sz, uint32* phys)
{
  return kmalloc_int(sz, 0, phys);
}

/*
  The function allocates an amount of memory, and returns the physical address
  which were allocated too.

  Input:
    The size to allocate and the start of the physical address.
  Output:
    A pointer to the allocater memory.
*/
uint32 kmalloc_ap(uint32 sz, uint32* phys)
{
  return kmalloc_int(sz, 1, phys);
}

/*
  The function allocates a pointer.

  Input:
    The size to allocate.
  Output:
    A pointer to the allocated memory.
*/
uint32 kmalloc(uint32 sz)
{
  return kmalloc_int(sz, 0, 0);
}

/*For reallocation of kmallocs, regular, a, p, ap: */
uint32 krealloc(uint32* ptr, uint32 old_sz, uint32 new_sz)
{
  uint32* out;
  out = (uint32*)kmalloc(new_sz);
  memory_copy((char*)out, (char*)ptr, old_sz);
  kfree(ptr);
  return (uint32)out;
}

uint32 krealloc_a(uint32* ptr, uint32 old_sz, uint32 new_sz)
{
  uint32* out;
  out = (uint32*)kmalloc_int(new_sz, 1, 0);
  memory_copy((char*)out, (char*)ptr, old_sz);
  kfree(ptr);
  return (uint32)out;
}

uint32 krealloc_ap(uint32* ptr, uint32 old_sz, uint32 new_sz, uint32* phys)
{
  uint32* out;
  out = (uint32*)kmalloc_int(new_sz, 1, phys);
  memory_copy((char*)out, (char*)ptr, old_sz);
  kfree(ptr);
  return (uint32)out;
}

uint32 krealloc_p(uint32* ptr, uint32 old_sz, uint32 new_sz, uint32* phys)
{
  uint32* out;
  out = (uint32*)kmalloc_int(new_sz, 0, phys);
  memory_copy((char*)out, (char*)ptr, old_sz);
  kfree(ptr);
  return (uint32)out;
}

/*
  The function expands a given heap.

  Input:
    The heap to expand and the new size for it.
  Output:
    None.
*/
void expand(uint32 new_size, heap_t* heap)
{
  if((new_size & 0xFFFFF000))
  {
    new_size &= 0xFFFFF000;
    new_size += 0x1000;
  }

  uint32 old_size = heap->end_address - heap->start_address;

  uint32 i = old_size;
  while(i < new_size)
  {
    alloc_frame(get_page(heap->start_address + i, 1, current_directory == 0 ? kernel_directory : current_directory),
                (heap->supervisor) ? 1 : 0, (heap->readonly) ? 0 : 1);
    i += 0x1000;  /* page size */
  }

  heap->end_address = heap->start_address + new_size;
}

/*
  The function contracts a given heap.

  Input:
    The heap to contract and the new size for it.
  Output:
    The new size of the heap.
*/
static uint32 contract(uint32 new_size, heap_t* heap)
{

  if (new_size&0x1000)
  {
    new_size &= 0x1000;
    new_size += 0x1000;
  }

  if (new_size < HEAP_MIN_SIZE)
    new_size = HEAP_MIN_SIZE;

  uint32 old_size = heap->end_address-heap->start_address;
  uint32 i = old_size - 0x1000;
  while (new_size < i)
  {
    free_frame(get_page(heap->start_address+i, 0, current_directory == 0 ? kernel_directory : current_directory));
    i -= 0x1000;
  }

  heap->end_address = heap->start_address + new_size;
  return new_size;
}

/*
  The function finds the smallest hole in a given heap.

  Input:
    The heap to find the smalles hole in, the minimum size for it and if it's page_aligned.
  Output:
    the address of the smallest hole in the given heap.
*/
static int32 find_smallest_hole(uint32 size, uint8 page_align, heap_t* heap)
{
  uint32 iterator = 0;
  while(iterator < heap->index.size)
  {
    header_t* header = (header_t *)lookup_ordered_array(iterator, &heap->index);

    if(!page_align)
    {
      uint32 location = (uint32)header;
      int32 offset = 0;
      if((location+sizeof(header_t)) & 0xFFFFF000 != 0)
        offset = 0x1000 /* page size */  - (location + sizeof(header_t)) % 0x1000;
      int32 hole_size = (int32)header->size - offset;

      if(hole_size >= (int32)size)
        break;
    }
    else if(header->size >= size)
    {
      break;
    }

    iterator++;
  }

  if(iterator == heap->index.size)
  {
    return -1;
  }
  else
  {
    return iterator;
  }
}

// An < operator for headers objects
static int8 header_t_less_than(void* a, void* b)
{
  return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

/*
  The function creates a new heap.

  Input:
    The heap's data.
  Output:
    The final heap.
*/
heap_t* create_heap(uint32 start, uint32 end_addr, uint32 max, uint8 supervisor, uint8 readonly)
{
  heap_t* heap = (heap_t*)kmalloc(sizeof(heap_t));

  heap->index = place_ordered_array((void*)start, HEAP_INDEX_SIZE, &header_t_less_than);

  start += sizeof(type_t) * HEAP_INDEX_SIZE;

  if(start & 0xFFFFF000 != 0)
  {
    start &= 0xFFFFF000;
    start += 0x1000;
  }

  heap->start_address = start;
  heap->end_address = end_addr;
  heap->max_address = max;
  heap->supervisor = supervisor;
  heap->readonly = readonly;

  header_t* hole = (header_t*)start;
  hole->size = end_addr - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;
  insert_ordered_array((void*)hole, &heap->index);

  return heap;
}

/*
  The function allocates a contiguous region of memory 'size' in size. If page_align==1, it creates that block starting
  on a page boundary.

  Input:
    The size to allocate and the parameters mentioned above.
  Output:
    None.
*/
void* alloc(uint32 size, uint8 page_align, heap_t* heap)
{
  uint32 new_size = size + sizeof(header_t) + sizeof(footer_t);
  // Find the smallest hole that will fit.
  int32 iterator = find_smallest_hole(new_size, page_align, heap);

  if(iterator == -1) // If we didn't find a suitable hole
  {
    uint32 old_length = heap->end_address - heap->start_address;
    uint32 old_end_address = heap->end_address;

    expand(old_length + new_size, heap);
    uint32 new_length = heap->end_address - heap->start_address;

    iterator = 0;
    uint32 idx = -1;
    uint32 value = 0x0;
    while(iterator < heap->index.size)
    {
      uint32 tmp = (uint32)lookup_ordered_array(iterator, &heap->index);
      if (tmp > value)
      {
        value = tmp;
        idx = iterator;
      }
      iterator++;
    }

    // If we didn't find ANY headers, we need to add one.
    if(idx == -1)
    {
      header_t* header = (header_t*)old_end_address;
      header->magic = HEAP_MAGIC;
      header->size = new_length - old_length;
      header->is_hole = 1;
      footer_t* footer = (footer_t*) (old_end_address + header->size - sizeof(footer_t));
      footer->magic = HEAP_MAGIC;
      footer->header = header;
      insert_ordered_array((void*)header, &heap->index);
    }
    else
    {
      header_t* header = lookup_ordered_array(idx, &heap->index);
      header->size += new_length - old_length;
      footer_t* footer = (footer_t*)((uint32)header + header->size - sizeof(footer_t));
      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }
    return alloc(size, page_align, heap);
  }

  header_t* orig_hole_header = (header_t*)lookup_ordered_array(iterator, &heap->index);
  uint32 orig_hole_pos = (uint32)orig_hole_header;
  uint32 orig_hole_size = orig_hole_header->size;

  if(orig_hole_size - new_size < sizeof(header_t) + sizeof(footer_t))
  {
    size += orig_hole_size - new_size;
    new_size = orig_hole_size;
  }

  if(page_align && orig_hole_pos & 0xFFFFF000)
  {
    uint32 new_location   = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - sizeof(header_t);
    header_t* hole_header = (header_t*)orig_hole_pos;
    hole_header->size     = 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - sizeof(header_t);
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;
    footer_t* hole_footer = (footer_t*) ((uint32)new_location - sizeof(footer_t));
    hole_footer->magic = HEAP_MAGIC;
    hole_footer->header = hole_header;
    orig_hole_pos = new_location;
    orig_hole_size = orig_hole_size - hole_header->size;
  }
  else
  {
    remove_ordered_array(iterator, &heap->index);
  }

  header_t* block_header = (header_t*)orig_hole_pos;
  block_header->magic = HEAP_MAGIC;
  block_header->is_hole = 0;
  block_header->size = new_size;
  footer_t* block_footer = (footer_t*) (orig_hole_pos + sizeof(header_t) + size);
  block_footer->magic = HEAP_MAGIC;
  block_footer->header = block_header;

  if(orig_hole_size - new_size > 0)
  {
    header_t *hole_header = (header_t *)(orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
    hole_header->magic    = HEAP_MAGIC;
    hole_header->is_hole  = 1;
    hole_header->size     = orig_hole_size - new_size;
    footer_t* hole_footer = (footer_t*)((uint32)hole_header + orig_hole_size - new_size - sizeof(footer_t) );
    if((uint32)hole_footer < heap->end_address)
    {
      hole_footer->magic = HEAP_MAGIC;
      hole_footer->header = hole_header;
    }
    insert_ordered_array((void*)hole_header, &heap->index);
  }

  return (void*)((uint32)block_header + sizeof(header_t));
}

/*
  The function releases a block allocated with 'alloc'.

  Input:
    A pointer to the block to free and to the heap to free the block from.
  Output:
    None.
*/
void free(void* p, heap_t* heap)
{
  if(!p)
  {
    return;
  }

  header_t* header = (header_t*)((uint32)p - sizeof(header_t));
  footer_t* footer = (footer_t*)((uint32)header + header->size - sizeof(footer_t));

  phys_mem_usage -= header->size;

  header->is_hole = 1;

  char do_add = 1;

  footer_t* test_footer = (footer_t*)((uint32)header - sizeof(footer_t));
  if(test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole)
  {
    uint32 cache_size = header->size; // Cache our current size.
    header = test_footer->header;     // Rewrite our header with the new one.
    footer->header = header;          // Rewrite our footer to point to the new header.
    header->size += cache_size;       // Change the size.
    do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
  }

  header_t* test_header = (header_t*)((uint32)footer + sizeof(footer_t) );
  if(test_header->magic == HEAP_MAGIC && test_header->is_hole)
  {
    uint32 iterator = 0;
    while((iterator < heap->index.size) && (lookup_ordered_array(iterator, &heap->index) != (void*)test_header))
      iterator++;

    if(iterator > heap->index.size)
    {
      test_header->magic = 0;
      test_header->is_hole = 1;
    }
    else
    {
      header->size += test_header->size;
      test_footer = (footer_t*)((uint32)test_header + test_header->size - sizeof(footer_t));
      footer = test_footer;

      remove_ordered_array(iterator, &heap->index);
    }
  }

  // If the footer location is the end address, we can contract.
  if((uint32)footer + sizeof(footer_t) == heap->end_address)
  {
    uint32 old_length = heap->end_address - heap->start_address;
    uint32 new_length = contract( (uint32)header - heap->start_address, heap);
    if(header->size - (old_length-new_length) > 0)
    {
      header->size -= old_length-new_length;
      footer = (footer_t*) ((uint32)header + header->size - sizeof(footer_t));
      footer->magic = HEAP_MAGIC;
      footer->header = header;
    }
    else
    {
      uint32 iterator = 0;
      while((iterator < heap->index.size) && (lookup_ordered_array(iterator, &heap->index) != (void*)test_header))
        iterator++;
      if(iterator < heap->index.size)
        remove_ordered_array(iterator, &heap->index);
    }
  }

  if(do_add == 1)
    insert_ordered_array((void*)header, &heap->index);
}

/*
  The function returns the amount of space the input pointer has alloced.

  Input:
    The allocated pointer to check.
  Output:
    The amount of allocated space.
*/
uint32 size_of_alloc(void* alloc)
{
  return *((uint32*)alloc - 1) -  (sizeof(header_t) + sizeof(footer_t));
}
