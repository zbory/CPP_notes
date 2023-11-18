## Move semantics

(1) https://www.youtube.com/watch?v=St0MNEU5b0o&t=389s

### Basics

* When transferring ownership, after copying the pointers of the previous owners pointer must be set to NULL, otherwise there would be two objects pointing to the same memory who don't know about each other and if one calls delete on it the other will have problems.

* **lvalue**: it has a name</br>
**rvalue**: it's a nameless (temporary) object</br>
**rvalue reference**: denoted with '&&', represent modifiable objects that are no longer needed

* Inside the function body the argument is treated as an lvalue, it has a name and exists for the whole scope not just for a single expression. Also if it were an rvlaue it would mplicitly trigger move semantics inside the function and would be unusable.
```cpp
Widget(Widget&& w) {
    // Here w is an lvalue
}
```

* Rvalue reference is used to distinguish the parameter of the two types of assignment operators.

* **std::move()**: indicates to treat the the argument as an rvalue, officially it is called an Xvalue (expiring value). It unconditionally casts its input into an rvalue reference, *it does not move anything*.

* On an object that has been moved from (**moved from object**), only three operations are advised: destructor (happens implicitly), copy assignment, move assignment (can be assigned to, to became a valid object again)

### New special member functions

Move constructor</br>
```cpp

class Widget
{
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};

public:
    Widget(Widget&& w) noexcept     // Core Guideline C.66
        : i(std::move(w.i))     // Correct, no speed advantage, still just copied.
                                // But a conventions to always move the data members in a move* function,
                                // provides an uniform look and if the type of i changes no need to change this part
        , s(std::move(w.s))     // Must be casted to an rvalue reference
        , pi(std::exchange(w.pi, nullptr))   // Optional fancy, returns the pointer value (of w.pi) and sets is to nullptr

    {
        // w.pi = nullptr;      // if std::exchange is not used
        w.pi = 0;               // Core Guideline C.64 - optional
    }
}
```
```
Note:

Core Guideline C.66: make move operations **noexcept**.</br></br>
strong exception safety guarantee: also known as commit or rollback semantics, operations can fail, but failed operations are guaranteed to have no side effects, leaving the original values intact.</br></br>

If a move operation is used in a function F providing the above guarantee - eg: std::vector::push_back - F must have the guarantee that the move operation inside it won't throw an exception so F won't fail and F can satisfy the exception safety guarantee of leaving the parameter object intact in case of fail. If the move operation could throw and exception while also modifying the parameter object the guarantee would be violated. If the move operation is not noexcept, the only way F can guarantee the strong exception safety if it falls back to copying instead of moving (to make sure the original parameter object is left intact in case of fail), so the performance gain from the move semantics is lost.  
```

Core Guideline F.15: [Prefer simple and conventional ways of passing information](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f15-prefer-simple-and-conventional-ways-of-passing-information)