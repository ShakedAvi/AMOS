#include "paging.h"

// The kernel's page directory
page_directory_t *kernel_directory = 0;

// The current page directory;
page_directory_t *current_directory = 0;

uint32 *frames;
uint32 nframes;

volatile uint32 memsize = 0; //size of paging memory

extern uint32 placement_address;
extern heap_t *kheap;

extern void copy_page_physical(uint32, uint32);

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32 frame_addr)
{
  uint32 frame = frame_addr / 0x1000;
  uint32 idx = INDEX_FROM_BIT(frame);
  uint32 off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32 frame_addr)
{
  uint32 frame = frame_addr/0x1000;
  uint32 idx = INDEX_FROM_BIT(frame);
  uint32 off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

static uint32 test_frame(uint32 frame_addr)
{
  uint32 frame = frame_addr/0x1000;
  uint32 idx = INDEX_FROM_BIT(frame);
  uint32 off = OFFSET_FROM_BIT(frame);
  return (frames[idx] & (0x1 << off));
}

static uint32 first_frame()
{
  uint32 i, j;
  for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
  {
    if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
    {
      // at least one bit is free here.
      for (j = 0; j < 32; j++)
      {
        uint32 toTest = 0x1 << j;
        if ( !(frames[i]&toTest) )
        {
          return i*4*8+j;
        }
      }
    }
  }
}


void virtual_map_pages(long addr, long size, uint32 rw, uint32 user)
{
  long i = addr;
  while (i < (addr + size + 0x1000))
  {
    if(i + size < memsize)
    {
      set_frame(first_frame());
      kmalloc(0x1000);
    }

    page_t *page = get_page(i, 1, current_directory);
    page->present = 1;
    page->rw = rw;
    page->user = user;
    page->frame = i / 0x1000;
    i += 0x1000;
  }
  return;
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
  if(page->frame)
  {
    return;
  }else{
    uint32 idx = first_frame();
    if (idx == (uint32)-1)
    {
      //PANIC! no free frames!!
    }
    set_frame(idx * 0x1000);
    page->present = 1;
    page->rw = (is_writeable == 1) ? 1 : 0;
    page->user = (is_kernel == 1) ? 0 : 1;
    page->frame = idx;
  }
}

void free_frame(page_t *page)
{
  uint32 frame;
  if (!(frame=page->frame))
  {
    return;
  }
  else
  {
    clear_frame(frame);
    page->frame = 0x0;
  }
}

void pageMem(uint32 location)
{
  uint32 j = location;
  while (j < location + (1024*768*4))
  {
    if(j + location + (1024*768*4) < memsize)
      set_frame(j); // Tell the frame bitmap that this frame is now used!
    //Get the page
    page_t *page = get_page(j, 1, kernel_directory);
    //And fill it
    page->present = 1;
    page->rw = 1;
    page->user = 1;
    page->frame = j / 0x1000;
    j += 0x1000;
  }
}

void init_paging(uint32 memorySize)
{
  uint32 mem_end_page = memorySize;
  memsize = memorySize;

  nframes = mem_end_page / 0x1000;
  frames = (uint32*)kmalloc(INDEX_FROM_BIT(nframes));
  memory_set(frames, 0, INDEX_FROM_BIT(nframes));

  kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
  memory_set((uint8*)kernel_directory, 0, sizeof(page_directory_t));
  kernel_directory->physicalAddr = (uint32)kernel_directory->tablesPhysical;

  uint32 vga_mem_addr = 0xFD000000; //replace me with a routine

  pageMem(vga_mem_addr);

  uint32 i = 0;
  for(i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
    get_page(i, 1, kernel_directory);

  i = 0;
  while (i < placement_address + 0x1000)
  {
    // Kernel code is readable but not writeable from userspace.
    alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }

  for(i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
    alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

  register_interrupt_handler(14, page_fault);
  // Enable paging.
  switch_page_directory(kernel_directory);

  // Initialise the kernel heap.
  kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_START + KHEAP_MAX_ADDRESS, 0, 0);
  expand(0x1000000, kheap); // Allocate some more space, 16MB

  current_directory = clone_directory(kernel_directory);
  switch_page_directory(current_directory);
}

void page_fault(registers_t *regs)
{
  char addr[10] = { 0 };
  asm volatile("sti");
  //init_timer(1000); 

  uint32 faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  uint32 present = !(regs->err_code & 0x1);   // Page not present
  uint32 rw = regs->err_code & 0x2;           // Write operation?
  uint32 us = regs->err_code & 0x4;           // Processor was in user-mode?
  uint32 reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  uint32 id = regs->err_code & 0x10;          // Caused by an instruction fetch?

  print("\nPage fault! ( ");
  if(present) {print("present ");}
  if(rw) {print("read-only ");}
  if(us) {print("user-mode ");}
  if(reserved) {print("reserved ");}
  print(") at ");
  int_to_ascii(faulting_address, addr);
  print(addr);
  print(" - EIP: ");
  int_to_ascii(regs->eip, addr);
  print(addr);
  print("\n");


  if(regs->eip != faulting_address)
  {
    print("Page fault caused by executing unpaged memory\n");
  }else{
    print("Page fault caused by reading unpaged memory\n");
  }
}

void switch_page_directory(page_directory_t *dir)
{
  current_directory = dir;
  asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));
  uint32 cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32 address, uint32 make, page_directory_t *dir)
{
  address /= 0x1000;
  uint32 table_idx = address / 1024;

  if(dir->tables[table_idx]) // If this table is already assigned
  {
    return &dir->tables[table_idx]->pages[address % 1024];
  }else if(make)
  {
    uint32 tmp;
    dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
    memory_set(dir->tables[table_idx], 0, 0x1000);
    dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
    return &dir->tables[table_idx]->pages[address % 1024];
  }else{
    return 0;
  }
}

static page_table_t *clone_table(page_table_t *src, uint32 *physAddr)
{
  page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
  memory_set((uint8*)table, 0, sizeof(page_table_t));

  int i = 0;
  for(i = 0; i < 1024; i++)
  {
    if (!src->pages[i].frame)
      continue;

    alloc_frame(&table->pages[i], 0, 0);

    if(src->pages[i].present) table->pages[i].present = 1;
    if(src->pages[i].rw)      table->pages[i].rw = 1;
    if(src->pages[i].user)    table->pages[i].user = 1;
    if(src->pages[i].accessed)table->pages[i].accessed = 1;
    if(src->pages[i].dirty)   table->pages[i].dirty = 1;

    copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
  }
  return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
  uint32 phys;
  page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
  memory_set((uint8*)dir, 0, sizeof(page_directory_t));

  uint32 offset = (uint32)dir->tablesPhysical - (uint32)dir;

  dir->physicalAddr = phys + offset;

  int i;
  for(i = 0; i < 1024; i++)
  {
    if(!src->tables[i])
      continue;

    if(kernel_directory->tables[i] == src->tables[i])
    {
      dir->tables[i] = src->tables[i];
      dir->tablesPhysical[i] = src->tablesPhysical[i];
    }else{
      uint32 phys;
      dir->tables[i] = clone_table(src->tables[i], &phys);
      dir->tablesPhysical[i] = phys | 0x07;
    }
  }
  return dir;
}
