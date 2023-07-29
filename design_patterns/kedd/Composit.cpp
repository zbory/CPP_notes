#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <numeric>

struct Component {
	virtual ~Component() = default;
	virtual std::string toString() const = 0;
};

class KeyVal : public Component {
    private:
		std::string key, value;
    public:
		KeyVal(const std::string &key, const std::string &value)
			: key{ key }, value{ value }
		{}
		std::string toString() const final {
			return '"' + key + "\": \"" + value + '"';
		}
};

class Dictionary : public Component {
    private:
		std::string name;
		std::vector<std::shared_ptr<Component>> comps;
    public:
		Dictionary(const std::string &name)
			: name{ name }
		{}
		Dictionary& add(const std::shared_ptr<Component> &c) {
			comps.push_back(c);
			return *this;
		}
		Dictionary& add(std::shared_ptr<Component> &&c) {
			comps.push_back(std::move(c));
			return *this;
		}

		std::string toString() const final {
			if (!comps.empty()) {
				return std::accumulate(
					std::next(std::cbegin(comps)), std::cend(comps),
					'"' + name + "\": {" + comps[0]->toString(),
					[](const std::string &res,
  					   const std::shared_ptr<Component> &curr) {
						return res + ", " + curr->toString();
					}
				) + "}";
			}
			return '"' + name + "\": {}";
		}
};

int main() {

	auto root = std::make_shared<Dictionary>("root");
	root->add(std::make_shared<KeyVal>("key", "value"));

	auto nested = std::make_shared<Dictionary>("nested");
	nested->add(std::make_shared<KeyVal>("key", "value"));

	root->add(nested);

	std::cout << root->toString() << '\n';
}