## constexpr

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