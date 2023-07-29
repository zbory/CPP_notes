#include <iostream>
#include <string>

class Name {
    private:
        std::string firstName;
        std::string lastName;

    public:

        Name(std::string firstName, std::string lastName)
            : firstName{ firstName }
            , lastName{ lastName }
        {}

        void print() const {
            std::cout << firstName << ", " << lastName << '\n';
        }
};

template<typename Printable>
struct RepeatPrint : public Printable {

    template<typename ...Args>
    RepeatPrint(Args&& ...args)
        : Printable(std::forward<Args>(args)...)
    {}

    void repeat(unsigned n) const {
        while (n-- > 0)
            this->print();
    }
};

template<typename Printable, typename ...Args>
auto make_repeater(Args&& ...args) {
    return RepeatPrint<Printable>{ std::forward<Args>(args)...};
}

int main() {
    auto rn = make_repeater<Name>("Antonio", "Banderas");
    rn.repeat(3);
}
