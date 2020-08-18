#include "paging.h"
#include "kheap.h"

page_directory_t *kernel_directory = 0;

page_directory_t *current_directory = 0;

uint32 *frames;
uint32 nframes;

extern uint32 placement_address;
extern heap_t *kheap;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32 frame_addr)
{
    uint32 frame = frame_addr/0x1000;
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

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return;
    }
    else
    {
        uint32 idx = first_frame();
        if (idx == (uint32)-1)
        {
            print("Error! No Free Frames!\n");
            asm("hlt");
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
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

void init_paging()
{
    uint32 mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;
    frames = (uint32*)kmalloc(INDEX_FROM_BIT(nframes));
    memory_set(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memory_set(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

    i = 0;
    while (i < placement_address+0x1000)
    {
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

    switch_page_directory(kernel_directory);

    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32 address, int make, page_directory_t *dir)
{
    address /= 0x1000;
    uint32 table_idx = address / 1024;

    if (dir->tables[table_idx])
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32 tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        memory_set(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}
