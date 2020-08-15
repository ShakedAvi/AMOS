#include "paging.h"

// A bitset of frames - used or free.
uint32 *frames;
uint32 nframes;

void init_paging()
{
  page_directory_t* current_directory;
  uint32 mem_end_page = 0x1000000;
  nframes = mem_end_page / 0x1000;
  frames = (uint32*)kmalloc(INDEX_FROM_BIT(nframes));
  memory_set(frames, 0, INDEX_FROM_BIT(nframes));

  kernel_directory = (page_directory_t*)kmalloc(sizeof(page_directory_t));
  memory_set((uint32*)kernel_directory, 0, sizeof(page_directory_t));
  current_directory = kernel_directory;

  int i = 0;
  while (i < placement_address)
  {
     // Kernel code is readable but not writeable from userspace.
     alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
     i += 0x1000;
  }

  loadPageDirectory(page_directory);
  enablePaging();
}

page_t *get_page(uint32 address, int make, page_directory_t *dir)
{
   address /= 0x1000;
   uint32 table_idx = address / 1024;
   if (dir->tables[table_idx]) // If this table is already assigned
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       uint32 tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_p(sizeof(page_table_t), &tmp);
       memory_set((uint32*)dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
}


// Static function to set a bit in the frames bitset
static void set_frame(uint32 frame_addr)
{
  uint32 frame = frame_addr/0x1000;
  uint32 idx = INDEX_FROM_BIT(frame);
  uint32 off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32 frame_addr)
{
  uint32 frame = frame_addr/0x1000;
  uint32 idx = INDEX_FROM_BIT(frame);
  uint32 off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32 test_frame(uint32 frame_addr)
{
  uint32 frame = frame_addr/0x1000;
  uint32 idx = INDEX_FROM_BIT(frame);
  uint32 off = OFFSET_FROM_BIT(frame);
  return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32 first_frame()
{
  uint32 i, j;
  for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
  {
      if (frames[i] != 0xFFFFFFFF)
      {
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

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
  {
      return;
  }
  else
  {
      uint32 idx = first_frame(); // idx is now the index of the first free frame.
      set_frame(idx*0x1000);
      page->present = 1;
      page->rw = (is_writeable)?1:0;
      page->user = (is_kernel)?0:1;
      page->frame = idx;
  }
}

// Function to deallocate a frame.
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
