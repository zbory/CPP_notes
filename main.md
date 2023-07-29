
- [Templates](#templates)
	- [Function templates](#function-templates)
	- [Class templates](#class-templates)
- [constexpr](#constexpr)
- [Working with serialized data](#working-with-serialized-data)
- [Tervezési minták](#tervezési-minták)


# Templates
## Function templates

* **typename** and **class** means the same, they are interchangeable,
T is just a placeholder

```cpp
template<typename T>

template<class T>
```

* **variadic template**: a class or function template that supports an arbitrary number of arguments
* **ellipsis (...)**:
for a function that takes a variable number of parameters, eg: findAverage(int count, ...)<br>
functions that use ellipsis must have at least one non-ellipsis parameter, the ellipsis must always be the last parameter<br>
include the cstdarg header, this header defines **va_list**, **va_arg**, **va_start** and **va_end** which are macros that we need to use to access the parameters<br>
https://www.learncpp.com/cpp-tutorial/ellipsis-and-why-to-avoid-them/


* templates usually have to be defined in a header file (recommended)
  * no inline necessary (?)<br>
* for functions, specifying the type is not mandatory, it will be (if possible) deduced from the argument type<br>
  
```cpp
int a,b;

myMax<int>(a,b)
myMax(a,b)
```

* C++20 introduces this usage, this is also/still a template definition

```cpp
void print(const auto& collection)
```

* Concepts

```cpp
template<typename T>
T myMax(T a, T b) {
	return b < a ? a : b;
}
```

There are two constraints in the example implicitly:<br>
operator < (returning bool)<br>
copy/move constructor<br>

To explicitly define these constraints: Concepts (C++20)

```cpp
template<typename T>
concept SupportLessThan = requires (T x) { x < x; };

template<typename T>
requires std::copyable<T> && SupportLessThan<T>
```

Results in  better, more understandable error messages for templated code

* the type of the return value is figured out by the compiler at compile time (C++14)

```cpp
template<typename T1, typename T2>
auto myMax(T1 a, T2 b)

auto a1 = myMax(0.7, 3); // called with different types
```

## Class templates

```cpp
template<typename T>
class Stack {
	std::vector<T> elems;

	void push(const& T);
	T pop();
	T top();
	void print();
}
```
In header files, no inline needed (?)<br>
generic member functions are only instantiated if used<br>
for the print function there is an implicit requirement (to support <<)<br>
if the << is not supported with the specific type, the code will still compile as long as print is not called<br>
it means we have mandatory and optional requirements<br>

* class template argument deduction **(CTAD)** (C++17)
 
```cpp
std::complex<int> c1(5, 3);	// all C++ versions
std::complex<int> c2{5, 3}	// since C++11
std::complex  c3{5, 3}		// deduces std::complex<int>

std::vector<int> v{0, 8, 15};
std::vector v2{0, 8, 15};	// deduces std::vector<int>
```

# constexpr

**DEF**: an object or function fit for use in constant expressions<br>
* the expression can be evaluated at compile time, eg: {3 + 4}
* compile-time constant is a constant whose value is known at compile-time, 'const' variables may or may not be compile-time constants
* a constexpr function or constructor is implicitly inline
* in many cases, compile-time constants will be optimized out of the program entirely
* runtime constants are constants whose initialization values aren’t known until runtime
* for a function to be fit for use in constant expressions, it must be explicitly declared constexpr
* PURPOSE: to ensure we get a compile-time const where we desire one
* in member function declarations. In C++11, constexpr implies const, while in C++14 and C++17 that is not the case
* a constexpr function is one whose return value is computable at compile time when consuming code requires it. Consuming code requires the return value at compile time to initialize a constexpr variable, or to provide a non-type template argument. When its arguments are constexpr values, a constexpr function produces a compile-time constant. When called with non-constexpr arguments, or when its value isn't required at compile time, it produces a value at run time like a regular function.
* a function declared with constexprs can only use objects and functions which "are also constexpr" IF it is required in a constexpr manner; if the function is not used in a constexpr manner anywhere in the code, it can use non-constexpr objects and functions and can be called with non-constexpr arguments
* https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170

# Working with serialized data
deserialization:
* DONT_1: struct to define data structure, allocate struct object, cast it to (char*) and memcpy/read buffer into the pointed location <-- compiler can do padding for non word length data members and wrongly offset the read data (solution can be #pragma pack(1) // gcc only )
* DONT_2: cast (reinterpret_cast) the read buffer data into the struct <-- can be undefined behaviour, see cppreference: reinterpret_cast
* POSIB_1: like reinterpret_cast but with bit_cast (C++20)

# [Tervezési minták](design_patterns/design_patterns.md)