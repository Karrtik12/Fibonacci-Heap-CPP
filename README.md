# Fibonacci Heap — C++ Implementation

A from-scratch implementation of the **Fibonacci Heap** data structure in C++, following the algorithms described in *Introduction to Algorithms* (CLRS, 3rd Edition), Chapter 19.

---

## What is a Fibonacci Heap?

A Fibonacci Heap is a priority queue data structure that achieves better amortized time bounds than binary or binomial heaps — particularly for `decrease-key`, which runs in **O(1) amortized** time. This makes it ideal for speeding up graph algorithms like **Dijkstra's shortest path** and **Prim's MST**.

The structure is a collection of min-heap-ordered trees stored in a doubly-linked circular root list. Its name comes from the Fibonacci sequence, which appears in the proof of its degree bounds.

---

## Project Structure

```
.
├── fheap.cpp                  # Driver program demonstrating all heap operations
├── fibonaci_heap_utils.h      # Struct definitions (node, fib_heap) and declarations
└── fibonaci_heap_utils.cpp    # Complete implementation of all operations
```

### Data Structures

**`node`** — Each element in the heap is a node with:
- `val` — the integer key stored in this node
- `deg` — number of children
- `left`, `right` — pointers forming a doubly-linked circular sibling list
- `parent`, `child` — pointers for the tree structure
- `child_lost` — mark bit, set to `true` when this node has lost a child (used by cascading cut)

**`fib_heap`** — The heap itself holds:
- `root` — entry point into the circular root list
- `min_pointer` — direct pointer to the minimum node (enables O(1) find-min)
- `n` — total number of nodes currently in the heap

---

## Operations

### `make_heap()` — Initialize
Creates and returns an empty Fibonacci Heap with `root = NULL`, `min_pointer = NULL`, and `n = 0`.

**Time Complexity:** O(1)

---

### `insert(heap, x)` — Insert
Allocates a new node with key `x` and adds it directly to the root list — no restructuring is done. The `min_pointer` is updated if `x` is smaller than the current minimum.

**Time Complexity:** O(1) amortized

```cpp
node* b = insert(heap, 10);
insert(heap, 20);
insert(heap, 30);
```

---

### `find_min(heap)` — Extract Minimum (Read-Only)
Returns the value of the minimum node by reading `heap->min_pointer->val` directly. Returns `-1` if the heap is empty.

**Time Complexity:** O(1)

```cpp
int min = find_min(heap);
```

---

### `delete_min(heap)` — Delete Minimum
Removes and returns the node pointed to by `min_pointer`. All children of the removed node are promoted to the root list, and `consolidate()` is called to restore the heap structure.

**Time Complexity:** O(log n) amortized

```cpp
node* removed = delete_min(heap);
```

---

### `decrease_key(heap, target, k)` — Decrease Key
Reduces the key of `target` to `k`. If this violates the min-heap property (i.e., `k` is less than the parent's key), the node is **cut** from its parent and moved to the root list. **Cascading cut** then propagates upward, cutting any marked ancestor until reaching an unmarked node or the root.

**Time Complexity:** O(1) amortized

```cpp
node* b = insert(heap, 10);
decrease_key(heap, b, 3);   // reduces node b's key from 10 to 3
```

---

### `delete_element(heap, target)` — Delete Arbitrary Node
Deletes any node from the heap by first calling `decrease_key(heap, target, NEG_INF)` (setting the key to `INT_MIN`) to float the node to the top, then calling `delete_min()` to remove it.

**`NEG_INF`** is defined as `-2147483647` (effectively `INT_MIN`) in `fibonaci_heap_utils.h`.

**Time Complexity:** O(log n) amortized

```cpp
delete_element(heap, target_node);
```

---

### `display(root)` / `display_tree(root, prefix, isLast)` — Visualize Heap
Prints the entire heap forest as an ASCII tree to stdout. Each node shows its value, degree, and whether its `child_lost` mark is set.

**Time Complexity:** O(n)

```
Fibonacci Heap Tree:

'-- 3 (deg=2)
    '-- 7 (deg=0)
    '-- 10 (deg=1)
        '-- 20 (deg=0)
-------------------------------
```

`display_tree()` is the recursive helper that handles indentation and tree-branch rendering. `display()` is the public-facing wrapper that prints the header and calls `display_tree()`.

---

## Internal / Helper Operations

### `consolidate(heap)`
Called after `delete_min()` to ensure no two roots in the root list share the same degree. Uses a degree-indexed array (`arr[]` of size `log₂(n) + 2`) to bin trees by degree. When a collision is found, the tree with the larger root is linked under the smaller one via `fib_heap_link()`, and the degree index is incremented. At the end, the minimum pointer is recomputed by scanning the consolidated array.

**Time Complexity:** O(log n) amortized

---

### `cut(heap, target, parent)`
Removes `target` from its parent's child list using `remove_from_child_list()`, adds it to the root list, clears its `parent` pointer, and resets its `child_lost` mark to `false`.

---

### `cascading_cut(heap, target)`
Walks up the tree from `target`. If the current node has not yet lost a child, it is marked (`child_lost = true`) and the walk stops. If it has already lost a child (already marked), it is cut and the walk continues with the grandparent. This ensures the degree bound — no node can lose more than one child without being promoted to the root list.

---

### `fib_heap_link(heap, target, new_parent)`
Makes `target` a child of `new_parent`: removes `target` from the root list, appends it to `new_parent`'s child list, increments `new_parent->deg`, and resets `target->child_lost` to `false`.

---

### `add_to_list(root, target)` / `remove_from_list(head, target)` / `remove_from_child_list(parent, target)`
Low-level linked-list utilities that insert or remove a node from a doubly-linked circular list, maintaining correct `left`/`right` pointers.

---

## Amortized Complexity Summary

| Operation          | Amortized Time |
|--------------------|---------------|
| `make_heap()`      | O(1)          |
| `insert()`         | O(1)          |
| `find_min()`       | O(1)          |
| `delete_min()`     | O(log n)      |
| `decrease_key()`   | O(1)          |
| `delete_element()` | O(log n)      |
| `consolidate()`    | O(log n)      |
| `display()`        | O(n)          |

---

## Build & Run

**Prerequisites:** A C++ compiler with C++11 or later (e.g., `g++`, `clang++`)

```bash
g++ -o fheap fheap.cpp fibonaci_heap_utils.cpp -lm
./fheap
```

---

## Reference

> Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009).
> *Introduction to Algorithms* (3rd ed.). MIT Press.
> **Chapter 19 — Fibonacci Heaps.**
