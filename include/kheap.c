#include "kheap.h"

// end is defined in the linker script.
extern uint32 end;
uint32 placement_address = (uint32)&end;
extern page_directory_t *kernel_directory;
heap_t *kheap=0;

uint32 kmalloc_int(uint32 sz, int align, uint32 *phys)
{
    if (kheap != 0)
    {
        void *addr = alloc(sz, (uint8)align, kheap);
        if (phys != 0)
        {
            page_t *page = get_page((uint32)addr, 0, kernel_directory);
            *phys = page->frame*0x1000 + (uint32)addr&0xFFF;
        }
        return (uint32)addr;
    }
    else
    {
        if (align == 1 && (placement_address & 0xFFFFF000) )
        {
            // Align the placement address;
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }
        if (phys)
        {
            *phys = placement_address;
        }
        uint32 tmp = placement_address;
        placement_address += sz;
        return tmp;
    }
}

void kfree(void *p)
{
    free(p, kheap);
}

uint32 kmalloc_a(uint32 sz)
{
    return kmalloc_int(sz, 1, 0);
}

uint32 kmalloc_p(uint32 sz, uint32 *phys)
{
    return kmalloc_int(sz, 0, phys);
}

uint32 kmalloc_ap(uint32 sz, uint32 *phys)
{
    return kmalloc_int(sz, 1, phys);
}

uint32 kmalloc(uint32 sz)
{
    return kmalloc_int(sz, 0, 0);
}

static void expand(uint32 new_size, heap_t *heap)
{
    // Get the nearest following page boundary.
    if (new_size&0xFFFFF000 != 0)
    {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    // This should always be on a page boundary.
    uint32 old_size = heap->end_address-heap->start_address;

    uint32 i = old_size;
    while (i < new_size)
    {
        alloc_frame( get_page(heap->start_address+i, 1, kernel_directory),
                     (heap->supervisor)?1:0, (heap->readonly)?0:1);
        i += 0x1000 /* page size */;
    }
    heap->end_address = heap->start_address+new_size;
}

static uint32 contract(uint32 new_size, heap_t *heap)
{
    // Get the nearest following page boundary.
    if (new_size&0x1000)
    {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    // Don't contract too far!
    if (new_size < HEAP_MIN_SIZE)
        new_size = HEAP_MIN_SIZE;

    uint32 old_size = heap->end_address-heap->start_address;
    uint32 i = old_size - 0x1000;
    while (new_size < i)
    {
        free_frame(get_page(heap->start_address+i, 0, kernel_directory));
        i -= 0x1000;
    }

    heap->end_address = heap->start_address + new_size;
    return new_size;
}

static int32 find_smallest_hole(uint32 size, uint8 page_align, heap_t *heap)
{
    // Find the smallest hole that will fit.
    uint32 iterator = 0;
    while (iterator < heap->index.size)
    {
        header_t *header = (header_t *)lookup_ordered_array(iterator, &heap->index);
        // If the user has requested the memory be page-aligned
        if (page_align > 0)
        {
            // Page-align the starting point of this header.
            uint32 location = (uint32)header;
            int32 offset = 0;
            if ((location+sizeof(header_t) & 0xFFFFF000) != 0)
                offset = 0x1000 /* page size */  - (location+sizeof(header_t))%0x1000;
            int32 hole_size = (int32)header->size - offset;
            // Can we fit now?
            if (hole_size >= (int32)size)
                break;
        }
        else if (header->size >= size)
            break;
        iterator++;
    }
    // Why did the loop exit?
    if (iterator == heap->index.size)
        return -1; // We got to the end and didn't find anything.
    else
        return iterator;
}

static int8 header_t_less_than(void*a, void *b)
{
    return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

heap_t *create_heap(uint32 start, uint32 end_addr, uint32 max, uint8 supervisor, uint8 readonly)
{
    heap_t *heap = (heap_t*)kmalloc(sizeof(heap_t));

    heap->index = place_ordered_array( (void*)start, HEAP_INDEX_SIZE, &header_t_less_than);

    start += sizeof(type_t)*HEAP_INDEX_SIZE;

    if (start & 0xFFFFF000 != 0)
    {
        start &= 0xFFFFF000;
        start += 0x1000;
    }
    heap->start_address = start;
    heap->end_address = end_addr;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    header_t *hole = (header_t *)start;
    hole->size = end_addr-start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    insert_ordered_array((void*)hole, &heap->index);

    return heap;
}

void *alloc(uint32 size, uint8 page_align, heap_t *heap)
{
    uint32 new_size = size + sizeof(header_t) + sizeof(footer_t);
    int32 iterator = find_smallest_hole(new_size, page_align, heap);

    if (iterator == -1)
    {
        uint32 old_length = heap->end_address - heap->start_address;
        uint32 old_end_address = heap->end_address;

        expand(old_length+new_size, heap);
        uint32 new_length = heap->end_address-heap->start_address;

        iterator = 0;
        uint32 idx = -1; uint32 value = 0x0;
        while (iterator < heap->index.size)
        {
            uint32 tmp = (uint32)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value)
            {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }

        if (idx == -1)
        {
            header_t *header = (header_t *)old_end_address;
            header->magic = HEAP_MAGIC;
            header->size = new_length - old_length;
            header->is_hole = 1;
            footer_t *footer = (footer_t *) (old_end_address + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            insert_ordered_array((void*)header, &heap->index);
        }
        else
        {
            header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size += new_length - old_length;
            footer_t *footer = (footer_t *) ( (uint32)header + header->size - sizeof(footer_t) );
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        return alloc(size, page_align, heap);
    }

    header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index);
    uint32 orig_hole_pos = (uint32)orig_hole_header;
    uint32 orig_hole_size = orig_hole_header->size;
    if (orig_hole_size-new_size < sizeof(header_t)+sizeof(footer_t))
    {
        size += orig_hole_size-new_size;
        new_size = orig_hole_size;
    }

    if (page_align && orig_hole_pos&0xFFFFF000)
    {
        uint32 new_location   = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
        header_t *hole_header = (header_t *)orig_hole_pos;
        hole_header->size     = 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        footer_t *hole_footer = (footer_t *) ( (uint32)new_location - sizeof(footer_t) );
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    }
    else
    {
        remove_ordered_array(iterator, &heap->index);
    }

    header_t *block_header  = (header_t *)orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    footer_t *block_footer  = (footer_t *) (orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;

    if (orig_hole_size - new_size > 0)
    {
        header_t *hole_header = (header_t *) (orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;
        footer_t *hole_footer = (footer_t *) ( (uint32)hole_header + orig_hole_size - new_size - sizeof(footer_t) );
        if ((uint32)hole_footer < heap->end_address)
        {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        insert_ordered_array((void*)hole_header, &heap->index);
    }

    return (void *) ( (uint32)block_header+sizeof(header_t) );
}

void free(void *p, heap_t *heap)
{
    if (p == 0)
        return;

    header_t *header = (header_t*) ( (uint32)p - sizeof(header_t) );
    footer_t *footer = (footer_t*) ( (uint32)header + header->size - sizeof(footer_t) );

    header->is_hole = 1;

    char do_add = 1;

    footer_t *test_footer = (footer_t*) ( (uint32)header - sizeof(footer_t) );
    if (test_footer->magic == HEAP_MAGIC &&
        test_footer->header->is_hole == 1)
    {
        uint32 cache_size = header->size; // Cache our current size.
        header = test_footer->header;     // Rewrite our header with the new one.
        footer->header = header;          // Rewrite our footer to point to the new header.
        header->size += cache_size;       // Change the size.
        do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
    }

    header_t *test_header = (header_t*) ( (uint32)footer + sizeof(footer_t) );
    if (test_header->magic == HEAP_MAGIC &&
        test_header->is_hole)
    {
        header->size += test_header->size; // Increase our size.
        test_footer = (footer_t*) ( (uint32)test_header + // Rewrite it's footer to point to our header.
                                    test_header->size - sizeof(footer_t) );
        footer = test_footer;
        uint32 iterator = 0;
        while ( (iterator < heap->index.size) &&
                (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
            iterator++;

        remove_ordered_array(iterator, &heap->index);
    }

    if ( (uint32)footer+sizeof(footer_t) == heap->end_address)
    {
        uint32 old_length = heap->end_address-heap->start_address;
        uint32 new_length = contract( (uint32)header - heap->start_address, heap);
        if (header->size - (old_length-new_length) > 0)
        {
            header->size -= old_length-new_length;
            footer = (footer_t*) ( (uint32)header + header->size - sizeof(footer_t) );
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            uint32 iterator = 0;
            while ( (iterator < heap->index.size) &&
                    (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
                iterator++;
            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }

    if (do_add == 1)
        insert_ordered_array((void*)header, &heap->index);
}
