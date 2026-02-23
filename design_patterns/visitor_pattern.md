## Visitor pattern

`std::variant` can hold multiple unrelated types, the active type is only determined at runtime. `std::visit` is like a type based (and type safe) switch case to handle all possible types. `std::visit` exhaustively forces at compile time that all the possible types in the variant are handled.

```cpp
#include <iostream>
#include <cstdint>
#include <variant>
#include <map>
#include <set>

// Old School Visitor
struct my_visitor {
    void operator()(std::int32_t v) {
        std::cout << 2 * v << '\n';
    }

    void operator()(std::string v) {
       std::cout << v  << "\nBut... The Cake is a Lie!\n";
    }
};

int main()
{
    std::variant<std::int32_t, std::string> data_variant;

    data_variant = "42";

    std::visit(my_visitor(), data_variant);

    // ---------------------------------------------------------

    // New Meta Visitor
    std::visit([](auto&& arg) {                                             // generic lambda that can take every type held by the variant, the compiler will generate a version for each type and std::visit will call them through function overloading
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::int32_t>) {                    // if constexpr: the branches where the condition evaluates to false are eliminated at compile time
            [](std::uint32_t v){std::cout << 2 * v << '\n';}(arg);
        } else if constexpr (std::is_same_v<T, std::string>) {
            [](const std::string& s){std::cout << s  << "\nBut... The Cake is a Lie!\n";}(arg);
        }
    }, data_variant);

    // ---------------------------------------------------------

    std::variant<std::map<std::uint16_t, std::string>, std::set<std::uint16_t>> container_variant;

    auto temp = new std::set<std::uint16_t>{2,4,8,16,32};
    container_variant = *temp;

    std::visit([](auto&& arg){
        std::cout << "is 4 present?: " << arg.contains(4) << '\n';          // if the contained types share the same functions it can be used with a generic lambda
    }, container_variant);

    return 0;
}
```

### Additional stuff
https://www.youtube.com/watch?v=3KyW5Ve3LtI

`std::visit` can be used with multiple instances of a variant:
```cpp
struct collision_resolver   // has to be able to handle all possible combinations
{
    void operator()(circle, circle) { /*  */ }
    void operator()(circle, box) { /*  */ }
    void operator()(box, circle) { /*  */ }
    void operator()(box, box) { /*  */ }
};

using my_variant = std variant<circle, box>;
my_variant v0{circle{}};
my_variant v1{box{}};

std visit(collision_resolver{}, v0, v1);
```