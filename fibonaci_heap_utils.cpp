#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

#include "fibonaci_heap_utils.h"

/*----------Subroutine Definations----------*/

node *remove_from_list(node *head, node *target)
{
    if (head == NULL || target == NULL)
        return head;

    if (target->right == target && target->left == target)
        return NULL;

    target->left->right = target->right;
    target->right->left = target->left;
    if (head == target)
        head = target->right;

    if (head->right == head)
    {
        head->left = head;
        head->right = head;
    }

    return head;
}

void remove_from_child_list(node *parent, node *target)
{
    if (parent == NULL || target == NULL || parent->child == NULL)
        return;

    if (parent->deg == 1)
    {
        parent->child = NULL;
        parent->deg = 0;
        return;
    }

    target->left->right = target->right;
    target->right->left = target->left;

    if (parent->child == target)
    {
        parent->child = target->right;
    }

    parent->deg--;

    target->left = target->right = NULL;
}

node *add_to_list(node *root, node *target)
{

    if (target == NULL)
        return root;

    if (root == NULL)
    {
        target->left = target;
        target->right = target;
        root = target;
        return root;
    }

    node *rightNode = root->right;

    root->right = target;
    target->left = root;
    target->right = rightNode;
    rightNode->left = target;
    return root;
}

void display_tree(node *root, const string &prefix, bool isLast)
{
    if (!root)
        return;

    node *curr = root;
    do
    {
        cout << prefix;

        cout << (isLast ? "'-- " : "|-- ");
        cout << curr->val << " (deg=" << curr->deg << ")" << (curr->child_lost ? " [lost]" : "") << "\n";

        string childPrefix = prefix + (isLast ? "    " : "|   ");

        if (curr->child != NULL)
        {
            display_tree(curr->child, childPrefix, true);
        }

        curr = curr->right;
        isLast = (curr == root); 
    } while (curr != root);
}

void display(node *root)
{
    cout << "Fibonacci Heap Tree:\n"
         << endl;
    if (root == nullptr)
    {
        cout << "Empty heap.\n";
        return;
    }
    display_tree(root);
    cout << "-------------------------------\n";
}

void fib_heap_link(fib_heap *heap, node *target, node *new_parent)
{
    heap->root = remove_from_list(heap->root, target);
    new_parent->child = add_to_list(new_parent->child, target);
    target->parent = new_parent;
    new_parent->deg++;
    target->child_lost = false;
}

void consolidate(fib_heap *heap)
{
    node *root = heap->root;
    int count = 0;
    do
    {
        count++;
        root = root->right;
    } while (root != heap->root);

    int max_degree = log2(heap->n) + 2;
    node *arr[max_degree];
    for (int i = 0; i < max_degree; i++)
        arr[i] = NULL;

    node *temp = heap->root;

    for (int z = 0; z < count; z++)
    {
        int d = temp->deg;
        node *next_temp = temp->right;
        while (arr[d] != NULL)
        {
            node *y = arr[d];
            if (temp->val > y->val)
            {
                node *t = temp;
                temp = y;
                y = t;
            }
            if (y == heap->root)
            {
                heap->root = temp;
            }
            fib_heap_link(heap, y, temp);
            arr[d] = NULL;
            d++;
        }
        arr[d] = temp;

        temp = next_temp;
    }

    heap->min_pointer = NULL;
    for (int i = 0; i < max_degree; i++)
    {
        if (arr[i] != NULL)
        {
            if (heap->min_pointer == NULL)
            {
                heap->root = arr[i];
                arr[i]->right = arr[i];
                arr[i]->left = arr[i];
                heap->min_pointer = arr[i];
            }
            else
            {
                node *newNode = arr[i];
                newNode->right = heap->root->right;
                newNode->left = heap->root;
                heap->root->right->left = newNode;
                heap->root->right = newNode;
                heap->root = arr[i];
                if (arr[i]->val < heap->min_pointer->val)
                {
                    heap->min_pointer = arr[i];
                }
            }
        }
    }
}

void cut(fib_heap *heap, node *target, node *parent)
{
    remove_from_child_list(parent, target);
    add_to_list(heap->root, target);
    target->parent = NULL;
    target->child_lost = false;
}

void cascading_cut(fib_heap *heap, node *target)
{
    node *target_parent = target->parent;
    if (target_parent != NULL)
    {
        if (target->child_lost == false)
            target->child_lost = true;
        else
        {
            cut(heap, target, target_parent);
            cascading_cut(heap, target_parent);
        }
    }
}

void decrease_key(fib_heap *heap, node *target, int k)
{
    if (target == NULL || k > target->val)
        return;
    target->val = k;
    node *target_parent = target->parent;

    if (target_parent != NULL && target->val < target_parent->val)
    {
        cut(heap, target, target_parent);
        cascading_cut(heap, target_parent);
    }
    if (target->val < heap->min_pointer->val)
        heap->min_pointer = target;
}

fib_heap* make_heap()
{
    fib_heap* heap = (fib_heap*)malloc(sizeof(fib_heap));
    heap->root = NULL;
    heap->min_pointer = NULL;
    heap->n = 0;
    return heap;
}

node *insert(fib_heap *heap, int x)
{
    node *insert_node = new node;

    insert_node->val = x;
    insert_node->left = heap->min_pointer;

    if (heap->min_pointer != NULL && heap->min_pointer->right)
        insert_node->right = heap->min_pointer->right;
    else
    {
        insert_node->right = insert_node;
        heap->root = insert_node;
        heap->min_pointer = insert_node;
    }

    (heap->min_pointer->right)->left = insert_node;
    heap->min_pointer->right = insert_node;
    if (x < heap->min_pointer->val)
        heap->min_pointer = insert_node;
    heap->n++;
    return insert_node;
}

int find_min(fib_heap *heap)
{
    if(heap->min_pointer != NULL){
        return heap->min_pointer->val;
    }else{
        return -1;
    }
}

node *delete_min(fib_heap *heap)
{
    node *minNode = heap->min_pointer;
    if (minNode != NULL)
    {
        node *ith_child = minNode->child;
        while (minNode->deg > 0)
        {
            if (ith_child->parent == NULL)
                break;
            node *ith_child_right = ith_child->right;
            add_to_list(heap->root, ith_child);
            ith_child->parent = NULL;
            ith_child = ith_child_right;
            minNode->deg--;
        }
        heap->root = remove_from_list(heap->root, minNode);
        if (minNode == minNode->right)
            heap->min_pointer = NULL;
        else
        {
            heap->min_pointer = minNode->right;
            consolidate(heap);
        }
        heap->n--;
        return minNode;
    }
    else
    {
        return NULL;
    }
}

void delete_element(fib_heap *heap, node *target)
{
    decrease_key(heap, target, NEG_INF);
    node *min_node = delete_min(heap);
    free(min_node);
}
