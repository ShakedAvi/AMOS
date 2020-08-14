#include "types.h"

static uint32 page_directory[1024] __attribute__((aligned(4096)));
static uint32 first_page_table[1024] __attribute__((aligned(4096)));

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void init_paging();
