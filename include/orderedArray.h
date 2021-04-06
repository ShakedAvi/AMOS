#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include "types.h"

typedef void* type_t;

typedef int8 (*lessthan_predicate_t)(type_t,type_t);
typedef struct oredered_array
{
    type_t *array;
    uint32 size;
    uint32 max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

#include "kheap.h"

// Implementation of < operator for "type_t"
int8 standard_lessthan_predicate(type_t a, type_t b);

/*
  The function creates a new ordered array object and returns it.

  Input:
    The maximum size of the oredered array and it's < operator function.
  Output:
    The final oredered array object.
*/
ordered_array_t create_ordered_array(uint32 max_size, lessthan_predicate_t less_than);

/*
  The function places memory inside an ordered array object.

  Input:
    The address of the memory to place inside the ordered array,
    it's maximum size and < operator function.
  Output:
    The final oredered array object.
*/
ordered_array_t place_ordered_array(void* addr, uint32 max_size, lessthan_predicate_t less_than);

/*
  The function destroys an ordered array object.

  Input:
    The ordered array object to destroy.
  Output:
    None.
*/
void destroy_ordered_array(ordered_array_t* array);

/*
  The function inserts a new element to an ordered array object.

  Input:
    The item to insert and the ordered array object.
  Output:
    None.
*/
void insert_ordered_array(type_t item, ordered_array_t* array);

/*
  The function looks up for and item inside an ordered array.

  Input:
    The index of the item to look up for and the ordered array to search inside.
  Output:
    The searched item.
*/
type_t lookup_ordered_array(uint32 i, ordered_array_t* array);

/*
  The function removes an item from an ordered array.

  Input:
    The index of the item to remove and the ordered array to remove it from.
  Output:
    None.
*/
void remove_ordered_array(uint32 i, ordered_array_t* array);

#endif
