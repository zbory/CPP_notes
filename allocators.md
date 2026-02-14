## Allocators

Allocators are concerned with allocating memory dynamically on the **heap**. They encapsulate strategies for access/addressing, allocation/deallocation of memory.<br> ( VS the **stack** is a contiguous block of memory. To "allocate" memory here, the CPU doesn't search for space; it simply moves a register called the Stack Pointer.)

### Standard Allocator
https://www.youtube.com/watch?v=LIb3L4vKZ7U


"**Evolution**": malloc -> new -> std::allocator

Overview of allocating storage:

| **Layer**        | **Tool / Mechanism**       | **Primary Purpose**     | **Memory Location** | **Key Characteristic**                                |
|------------------|----------------------------|-------------------------|---------------------|-------------------------------------------------------|
| **High Level**   | std::vector, std::string   | Automatic Resource Mgmt | Heap                | Uses RAII; handles its own new/delete.                |
| **Abstraction**  | std::allocator<T>          | Policy-based Allocation | Heap                | Separates memory allocation from object construction. |
| **Standard C++** | new / delete               | Object Lifecycle Mgmt   | Heap                | Allocates bytes and calls constructors/destructors.   |
| **Standard C**   | (std::)malloc / free         | Raw Byte Allocation     | Heap                | No type awareness; no constructors called.            |
| **Automatic**    | Local Variable Declaration | Scope-based Storage     | Stack               | Managed by CPU; near-zero overhead.                   |
| **Kernel Level** | mmap, brk, sbrk            | System Resource Grant   | System RAM          | The OS handing "pages" of memory to the process.    |


**History**: std::allocator was not created for memory allocation but to manage the "near" and "far" memory (fix instead of near/far pointers), for example the primary ~640kb memory and a 1MB extension of a PC in the days of DOS

Allocation is only concerned with **size** and **alignment**, doesn't care about type, for example memory for std::uint16_t and std::int16_t is not allocated differently.

The std::allocator is **stateless**. It doesn’t need to store any data (like a pointer to a specific heap) because it is hard-coded to always talk to the global `operator new`. Because it has no unique "identity" or specific memory pool it belongs to, an std::vector fox example can just create one on the fly whenever it needs to.<br>
In contrast for example the boost::interprocess::allocator is a **stateful** allocator. It must know which specific shared memory segment it is supposed to manage.


It's not a factory. The *allocation* of the memory and the *object creation* are separate. (Even though until c++20, std:allocator did have a construct()/destroy() method)
```cpp
#include <memory>   // std:allocator, std::construct_at, std::destroy_at

struct A{
    A() {
        std::cout << "A constructed\n";
    }

    ~A() {
        std::cout << "A destructed\n";
    }
    std::uint16_t data{0};
};

// ...
    std::allocator<A> allocator;
    A* array = allocator.allocate(5);

    for(size_t i{0}; i < 5; i++) {
        std::construct_at(&array[i]);
    }

    for(size_t i{0}; i < 5; i++) {
        std::destroy_at(&array[i]);
    }

    allocator.deallocate(array, 5);
```
Some ideas for custom allocators:<br>
**Freelist**: keeps tab of deallocated blocks in a list and instead of freeing them it can use them for the next allocate request<br>
**Prefix/Postfix**: builds on top of another allocator but adds some info before/after the allocated block for debug purposes
### Custom allocators

https://www.youtube.com/watch?v=RpD-0oqGEzE

### Memory Management

Some motivation to manage memory "by hand":<br>
* new can actually be costly
* memory chaos (fragmented memory)
* cache locality

What we can gain:
* nearly instananeous allocation
* eliminate fragmentation
* optimal data layout for CPU cache


`std::vector` actually looks like this with a default second parameter
```cpp
std::vector<std::uint8_t, std::allocator<std::uint8_t>>
```




Further learning: `allocator_traits`