#include <iostream>
#include <string>
#include <variant>


struct JsonValueVisitor {

	void operator()(const std::string& val) const {
		std::cout << '"' << val << '"';
	}

	void operator()(std::nullptr_t ) const {
		std::cout << "NUL";
	}

	template<typename T>
	void operator()(T t) const {
		std::cout << t;
	}
};



int main() {
	using namespace std::literals;
	std::variant<int, double, std::string, std::nullptr_t> v = nullptr;

	std::visit(JsonValueVisitor{}, v);
}
