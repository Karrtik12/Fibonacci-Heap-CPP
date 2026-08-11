#ifndef FIBONACI_HEAP_UTILS_H
#define FIBONACI_HEAP_UTILS_H

#define NEG_INF -2147483647

typedef struct _node
{
    int val;
    int deg = 0;
    struct _node *left = this;
    struct _node *parent = NULL;
    struct _node *right = this;
    struct _node *child = NULL;
    bool child_lost = false;
} node;

typedef struct _fib_heap
{
    struct _node *root;
    struct _node *min_pointer;
    int n;
} fib_heap;

/*----------Subroutine Declarations----------*/

/*-----Main Subroutines-----*/

fib_heap* make_heap();

node* insert(fib_heap *heap, int x);

int find_min(fib_heap *heap);

void decrease_key(fib_heap *heap, node *target, int k);

node *delete_min(fib_heap *heap);

/*-----Helper Subroutines-----*/

node *remove_from_list(node *head, node *target);

void remove_from_child_list(node *parent, node *target);

node *add_to_list(node *root, node *target);

void display_tree(node *root, const std::string &prefix = "", bool isLast = true);

void display(node *root);

void fib_heap_link(fib_heap *heap, node *target, node *new_parent);

void consolidate(fib_heap *heap);

void cut(fib_heap *heap, node *target, node *parent);

void cascading_cut(fib_heap *heap, node *target);

void delete_element(fib_heap *heap, node *target);

#endif