#include <iostream>
#include <string>
#include <type_traits>


template<typename Printable>
struct RepeatPrint {

	//using SubType = Printable;

	void repeat(unsigned n) {
		const Printable &der = static_cast<Printable&>(*this);
		while (n-- > 0) {
			if constexpr (std::is_invocable<
				decltype(&Printable::print),
				const Printable*,
				int>::value)
				der.print(n);
			else
			    der.print();
		}
	}
};

class Name1 : public RepeatPrint<Name1> {

	//static_assert(std::is_same<Name1, SubType>::value, "OK");

    private:
		std::string firstName;
		std::string lastName;
    public:
		Name1(std::string firstName, std::string lastName)
			: firstName{ std::move(firstName) }
			, lastName{ std::move(lastName) }
		{}

		void print() const {
			std::cout << lastName << ", " << firstName << '\n';
		}
};

class Name2 : public RepeatPrint<Name2> {

	//static_assert(std::is_same<Name1, SubType>::value, "OK");

    private:
	    std::string firstName;
	    std::string lastName;
    public:
	    Name2(std::string firstName, std::string lastName)
		    : firstName{ std::move(firstName) }
		    , lastName{ std::move(lastName) }
	    {}

	    void print(int i) const {
		    std::cout << i << ": " << lastName << ", " << firstName << '\n';
	    }
};

int main() {

	{
		Name1 ned{ "Antonio", "Banderas" };
		ned.repeat(3);
	}

	{
		Name2 ned{ "Antonio", "Banderas" };
		ned.repeat(3);
	}

}

