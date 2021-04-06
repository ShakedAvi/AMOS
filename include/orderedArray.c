#include "orderedArray.h"

// Implementation of < operator for "type_t"
int8 standard_lessthan_predicate(type_t a, type_t b)
{
    return (a<b)?1:0;
}

/*
  The function creates a new ordered array object and returns it.

  Input:
    The maximum size of the oredered array and it's < operator function.
  Output:
    The final oredered array object.
*/
ordered_array_t create_ordered_array(uint32 max_size, lessthan_predicate_t less_than)
{
    ordered_array_t to_ret;
    to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));
    memory_set(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

/*
  The function places memory inside an ordered array object.

  Input:
    The address of the memory to place inside the ordered array,
    it's maximum size and < operator function.
  Output:
    The final oredered array object.
*/
ordered_array_t place_ordered_array(void* addr, uint32 max_size, lessthan_predicate_t less_than)
{
    ordered_array_t to_ret;
    to_ret.array = (type_t*)addr;
    memory_set(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

/*
  The function destroys an ordered array object.

  Input:
    The ordered array object to destroy.
  Output:
    None.
*/
void destroy_ordered_array(ordered_array_t* array)
{
    kfree(array->array);
}

/*
  The function inserts a new element to an ordered array object.

  Input:
    The item to insert and the ordered array object.
  Output:
    None.
*/
void insert_ordered_array(type_t item, ordered_array_t* array)
{
    uint32 iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;
    if (iterator == array->size)
        array->array[array->size++] = item;
    else
    {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size)
        {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

/*
  The function looks up for and item inside an ordered array.

  Input:
    The index of the item to look up for and the ordered array to search inside.
  Output:
    The searched item.
*/
type_t lookup_ordered_array(uint32 i, ordered_array_t* array)
{
    return array->array[i];
}

/*
  The function removes an item from an ordered array.

  Input:
    The index of the item to remove and the ordered array to remove it from.
  Output:
    None.
*/
void remove_ordered_array(uint32 i, ordered_array_t* array)
{
    while (i < array->size)
    {
        array->array[i] = array->array[i+1];
        i++;
    }
    array->size--;
}
