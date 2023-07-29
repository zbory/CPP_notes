#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class Color {red, green , blue};
enum class Size {small, medium, large};

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

struct Specification {
    virtual ~Specification() = default;
    virtual bool isSatisfied(const Product&) const = 0;
};

struct ColorSpecification : public Specification {
    private:
        Color color;
    public:
        ColorSpecification(Color color) : color{ color } {}
        bool isSatisfied(const Product &product) const final {
            return product.color == color;
        }
};

struct SizeSpecification : public Specification {
    private:
        Size size;
    public:
        SizeSpecification(Size size) : size{ size } {}
        bool isSatisfied(const Product &product) const final {
            return product.size == size;
        }
};

class AndSpecification : public Specification {
    private:
        const Specification &lhs;
        const Specification &rhs;
    public:
        AndSpecification(const Specification &lhs,
                         const Specification &rhs)
            : lhs{ lhs }, rhs{ rhs }
        {}

        bool isSatisfied(const Product &product) const {
            return lhs.isSatisfied(product) &&
                   rhs.isSatisfied(product);
        }
};

struct ProductFilter {
    std::vector<Product*>
    filter(std::vector<std::unique_ptr<Product>> &prods,
           const Specification &spec) {
        auto res = std::vector<Product*>{};
        for (const auto &e : prods) {
            if (spec.isSatisfied(*e))
                res.push_back(e.get());
        }
        return res;
    }
};

int main() {
    std::vector<std::unique_ptr<Product>> storage;

    storage.push_back(
        std::make_unique<Product>("Apple",
                                  Color::green,
                                  Size::small));
    storage.push_back(
        std::make_unique<Product>("Tree",
                                  Color::green,
                                  Size::large));
    storage.push_back(
        std::make_unique<Product>("House",
                                  Color::blue,
                                  Size::large));


    ProductFilter pf;

    ColorSpecification cspec{ Color::green };
    SizeSpecification sspec{ Size::large };
    AndSpecification aspec{ cspec, sspec };

    const auto green_and_large_things =
                                  pf.filter(storage, aspec);
    for (const auto &x : green_and_large_things) {
        std::cout << x->name << '\n';
    }


}


