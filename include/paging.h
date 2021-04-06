#ifndef PAGING_H
#define PAGING_H

#include "types.h"
#include "kheap.h"
#include "isr.h"
#include "timer.h"

#define KHEAP_MAX_ADDRESS 0xFFFF000

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
    page_table_t* tables[1024];

    uint32 tablesPhysical[1024];

    uint32 physicalAddr;
} page_directory_t;


extern page_directory_t* current_directory;

/*
  The function sets up the environment, page directories etc and
  enables paging.

  Input:
    The size of the memory to enable paging for.
  Output:
    None.
*/
void init_paging(uint32 memory_size);

/*
  The function causes a specified page directory to be loaded into the
  CR3 register.

  Input:
    The page directory to load.
  Output:
    None.
*/
void switch_page_directory(page_directory_t* new);

/*
  The function retrieves a pointer to the page required.
  If make == 1, if the page-table in which this page should
  reside isn't created, create it

  Input:
    The address of the page to get, if the caller wants to make it and it's page directory.
  Output:
    The required page.
*/
page_t* get_page(uint32 address, uint32 make, page_directory_t* dir);

/*
  The function makes a copy of a page directory.

  Input:
    The page directory to clone.
  Output:
    The copy of the given page directory.
*/
page_directory_t* clone_directory(page_directory_t* src);

/*
  The function maps virtual pages.

  Input:
    The address to start mapping from, the size to map and any page's rw and user parameters.
  Output:
    None.
*/
void virtual_map_pages(long addr, long size, uint32 rw, uint32 user);

/*
  The function allocates a new frame to a page.

  Input:
    The page to allocate a new frame to and it's user and rw parameters.
  Output:
    None.
*/
void alloc_frame(page_t* page, int is_kernel, int is_writeable);

/*
  The function frees the frame of a given page.

  Input:
    The page to free it's frame.
  Output:
    None.
*/
void free_frame(page_t* page);

/*
  The function is an handler to page fault exception.

  Input:
    The registers when the exception occurred.
  Output:
    None.
*/
void page_fault(registers32_t* regs);

#endif //PAGING_H
