#ifndef PAGING_H
#define PAGING_H

#include "types.h"
#include "isr.h"
#include "kheap.h"

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern uint32 placement_address;
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

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

page_directory_t* kernel_directory;

static uint32 page_directory[1024] __attribute__((aligned(4096)));
static uint32 first_page_table[1024] __attribute__((aligned(4096)));

void init_paging();
page_t *get_page(uint32 address, int make, page_directory_t *dir);

static void set_frame(uint32 frame_addr);
static void clear_frame(uint32 frame_addr);
static uint32 test_frame(uint32 frame_addr);
static uint32 first_frame();
void alloc_frame(page_t *page, int is_kernel, int is_writeable);
void free_frame(page_t *page);

#endif
