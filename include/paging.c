#include "paging.h"

void init_paging()
{
  unsigned int i = 0;

  for(i = 0; i < 1024; i++)
  {
    page_directory[i] = 0x00000002;
  }

  for(i = 0; i < 1024; i++)
  {
    first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
  }

  page_directory[0] = ((unsigned int)first_page_table) | 3;

  loadPageDirectory(page_directory);
  enablePaging();
}
