#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class Color { red, green, blue };
enum class Size { small, medium, large };

struct Product {
    std::string name;
    Color color;
    Size size;

    Product(const std::string &name, Color color, Size size)
        : name{ name }
        , color{ color }
        , size{ size }
    {}
};

struct ISpecification {
    virtual ~ISpecification() = default;
    virtual bool isSatisfied(const Product&) const = 0;
};

class ColorSpecification : public ISpecification {
    private:
        Color color;
    public:
        ColorSpecification(Color color)
            : color{ color }
        {}
        
        bool isSatisfied(const Product &prod) const {
            return prod.color == color;
        }
};

class SizeSpecification : public ISpecification {
    private:
        Size size;
    public:
        SizeSpecification(Size size)
            : size{ size }
        {}

        bool isSatisfied(const Product &prod) const {
            return prod.size == size;
        }
};

template<typename T>struct Tag {
    using Type = T;
};

class Specification {
    private:
        std::shared_ptr<ISpecification> impl;
    public:
        template<typename Tag, typename ...Args>
        Specification(Tag, Args&& ...args)
            : impl{ std::make_shared<Tag::Type>(std::forward<Args>(args)...) }
        {}

        bool isSatisfied(const Product &prod) const {
            return impl->isSatisfied(prod);
        }
};

template<typename T, typename ...Args>
auto make_specification(Args&& ...args) {
    return Specification(Tag<T>{}, std::forward<Args>(args)...);
}

class AndSpecification : public ISpecification{
    private:
        Specification lhs;
        Specification rhs;
    public:

        AndSpecification(const Specification &lhs, const Specification &rhs)
            : lhs{ lhs }, rhs{ rhs }
        {}

        bool isSatisfied(const Product &prod) const {
            return lhs.isSatisfied(prod) && rhs.isSatisfied(prod);
        }
};

auto filter(const std::vector<Product> &prod, const Specification &spec) {
    std::vector<Product> res;
    for (const auto &e : prod)
        if (spec.isSatisfied(e))
            res.push_back(e);
    return res;
}

int main() {

    const std::vector<Product> storage{
        { "Apple", Color::green, Size::small },
        { "Tree",  Color::green, Size::large },
        { "House", Color::blue,  Size::large }
    };

    auto isGreen = make_specification<ColorSpecification>(Color::green);
    auto isLarge = make_specification<SizeSpecification>(Size::large);
    auto isLargeAndGreen = make_specification<AndSpecification>(isLarge, isGreen);

    auto largeAndGreenThings = filter(storage, isLargeAndGreen);
    for (const auto &e : largeAndGreenThings)
        std::cout << e.name << '\n';
}