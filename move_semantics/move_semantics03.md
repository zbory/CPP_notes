## NVRO/Copy elision

### Example code

```cpp
#include <iostream>

struct A {
    std::string data{"The Cake is a Lie\n"};

    A() { std::cout << "Constructor called\n"; }
    A(const A& a) {
        data = a.data;
        std::cout << "Copy constructor called\n"; 
    }
    A& operator=(const A& a) {
        data = a.data;
        std::cout << "Copy assign called\n";
        return *this;
    }
    A(A&& a) {
        data = std::move(a.data);
        std::cout << "Move constructor called\n";}
    A& operator=(A&& a) {
        data = std::move(a.data);
        std::cout << "Move assignment called\n";
        return *this;
    }
    ~A() { std::cout << "Destructor called\n"; }
};

A foo() {
    A a;
    return a;
}

int main() {
    std::cout << "before foo()\n";
    A a = foo();
    std::cout << "after foo()\n";

    return 0;
}
```

## C++98
`g++ -o0 -std=c++98 -fno-elide-constructors -o main main.cpp`

**Output**:

```cpp
before foo()
Constructor called, addr: 0x16b596ed0      // Creating local A object in foo()
Copy constructor called, addr: 0x16b596f18 // Copy-create a returned temporary A object into the callers scope (main)
Destructor called, addr: 0x16b596ed0       // Destroy local A object in foo()
Copy constructor called, addr: 0x16b596f30 // Copy-create the local A from the temporary in main
Destructor called, addr: 0x16b596f18       // Destroy temporary returned A object
after foo()
Destructor called, addr: 0x16b596f30       // End of main(), destroy 'a'
```

## C++11
`g++ -o0 -std=c++11 -fno-elide-constructors -o main main.cpp`
<br>
<br>The same as C++98 only copy operations are replaced with move operations.

**Output**:

```cpp
before foo()
Constructor called, addr: 0x16ef56ed0       // Creating A object in foo()
Move constructor called, addr: 0x16ee86f18  // Move-create a returned temporary A object into the callers scope (main)
Destructor called, addr: 0x16ee86ed0        // Destroy local A object in foo()
Move constructor called, addr: 0x16ee86f30  // Move-create the local A from the temporary in main
Destructor called, addr: 0x16ee86f18        // Destroy temporary returned A object
after foo()
Destructor called, addr: 0x16ee86f30        // End of main(), destroy 'a'
```

## C++17
`g++ -o0 -std=c++17 -fno-elide-constructors -o main main.cpp`
<br>
<br>Guaranteed copy elision, specifically, for the prvalue (pure rvalue) temporary returned by foo(). The standard guarantees that the temporary return value will be constructed directly into main's a. This means there is no second move constructor call and no destructor for a temporary return object. The temporary object itself is "elided".<br>
**NRVO**: The elision of the move from the named local object to the return value slot is still optional. Since `-fno-elide-constructors` was used, the compiler is forced not to do it.

**Output**:

```cpp
before foo()
Constructor called, addr: 0x16fd96ee0       // Creating A object in foo()
Move constructor called, addr: 0x16fd96f30  // Move from foo()'s local 'a' to the 'a' in main
Destructor called, addr: 0x16fd96ee0        // Destroy local A object in foo()
after foo()
Destructor called, addr: 0x16fd96f30        // End of main(), destroy 'a'
```

**With Copy Elision enabled**<br><br>
`g++ -o0 -std=c++17 -o main main.cpp`
<br>
<br>Because of **Guaranteed Copy Elision (specifically NRVO)**, the compiler sees that the local object `a` in `foo()` is immediately returned by value. Instead of constructing a separate local `A` object within `foo()` and then moving/copying it, the compiler constructs this `A` object *directly into the memory location* that `main` has reserved for its `A a` object.

**Output**:

```cpp
before foo()
Constructor called, addr: 0x16fbeaf30       // Creating A object directly in main's context
after foo()
Destructor called, addr: 0x16fbeaf30        // End of main(), destroy 'a'
```