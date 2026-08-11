#include <iostream>
#include <string.h>
#include <math.h>

#include "fibonaci_heap_utils.h"      // contains all the subroutines related to fibonaci heap

using namespace std;

int main()
{
    fib_heap *heap = make_heap();
    node* b = insert(heap, 10);
    insert(heap, 20);
    insert(heap, 30);
    insert(heap, 40);
    // insert(heap, 24);
    // insert(heap, 18);
    // insert(heap, 52);
    // insert(heap, 38);
    // insert(heap, 30);
    // insert(heap, 26);
    // insert(heap, 46);
    // // node* a = insert(heap, 107);
    // insert(heap, 41);
    // insert(heap, 35);
    display(heap->min_pointer);
    delete_min(heap);
    // consolidate(heap);
    display(heap->min_pointer);
    insert(heap, 5);
    insert(heap, 7);
    delete_min(heap);
    decrease_key(heap, b, 3);
    display(heap->min_pointer);
    // delete_element(heap, a);
    // display(heap->min_pointer);

    // delete_min(heap);
    // display(heap->min_pointer);
    // decrease_key(heap, a, 7);
    // find_min(heap);

    return 0;
}