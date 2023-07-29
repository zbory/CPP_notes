#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum class Color { red, green, blue};
enum class Size { small, medium, large};

struct Product {
	std::string name;
	Color color;
	Size size;
};

int main() {

	const std::vector<Product> storage{
		{ "Apple", Color::green, Size::small },
		{ "Tree",  Color::green, Size::large },
		{ "House", Color::blue,  Size::large }
	};

	auto colorFilter = [](Color color) {
		return [color](const Product &prod) {
			return prod.color == color;
		};
	};

	auto sizeFilter = [](Size size) {
		return [size](const Product &prod) {
			return prod.size == size;
		};
	};

	auto greenFilter = colorFilter(Color::green);
	auto largeFilter = sizeFilter(Size::large);

	auto andFilter = [](auto &lhs, auto &rhs) {
		return [lhs, rhs](const Product &prod) {
			return lhs(prod) && rhs(prod);
		};
	};

	std::vector<Product> greenAndLargeThings;
	std::copy_if(
		std::cbegin(storage), std::cend(storage),
		std::back_inserter(greenAndLargeThings),
		andFilter(greenFilter, largeFilter)
	);

	for (const auto &e : greenAndLargeThings)
		std::cout << e.name << " is green and large\n";
}


