#ifndef PAGING_H
#define PAGING_H

#include "types.h"
#include "kheap.h"
#include "isr.h"

typedef struct page
{
    uint32 present    : 1;   // Page present in memory
    uint32 rw         : 1;   // Read-only if clear, readwrite if set
    uint32 user       : 1;   // Supervisor level only if clear
    uint32 accessed   : 1;   // Has the page been accessed since last refresh?
    uint32 dirty      : 1;   // Has the page been written to since last refresh?
    uint32 unused     : 7;   // Amalgamation of unused and reserved bits
    uint32 frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    page_table_t *tables[1024];

    uint32 tablesPhysical[1024];

    uint32 physicalAddr;
} page_directory_t;

void init_paging();

void switch_page_directory(page_directory_t *new);

page_t *get_page(uint32 address, int make, page_directory_t *dir);

void alloc_frame(page_t *page, int is_kernel, int is_writeable);
void free_frame(page_t *page);


#endif
