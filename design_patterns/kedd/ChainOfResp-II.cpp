#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <type_traits>
#include <map>

struct Request {
	std::string name;
	std::string password;

	std::string recourse;
	std::string payload;
};


struct Middleware {
	virtual ~Middleware() = default;
	virtual bool handle(const Request&) const = 0;
};

class MiddlewareChain {

	private:

		std::vector<std::unique_ptr<Middleware>> chain;

    public:

		template<typename M, typename ...Args>
		MiddlewareChain& append(Args&&... args) {

			static_assert(std::is_base_of<Middleware, M>::value,
				          "Wrong middleware.");

			chain.push_back(
				std::make_unique<M>(
					std::forward<Args>(args)...
				)
			);

			return *this;
		}

		bool handle(const Request &req) const {
			for (const auto &mw : chain) {
				if (!mw->handle(req))
					return false;
			}
			return true;
		}

};

struct Authentication : public Middleware {
    private:

		const auto& getUserDB() const {
			static const std::map<std::string, std::string> userDB{
				{ "Willy", "12345" },
				{ "Wonka", "csoki" }
			};

			return userDB;
		}

		bool isValid(const std::string name, const std::string passwd) const {
			if (const auto it = getUserDB().find(name);
				it != getUserDB().cend()) {
				return it->second == passwd;
			}
			return false;
		}

    public:
		Authentication() = default;
		bool handle(const Request &req) const {
			if (!isValid(req.name, req.password))
				return false;
			return true;
		}
};

class Foo {};

int main() {

	auto req = Request{ "Willy", "12345", "index.html", "almafa=kortefa" };

	MiddlewareChain mq;
	mq.append<Authentication>();
	mq.append<Authentication>();
	//mq.append<Foo>();

	std::cout << "res: " << mq.handle(req) << '\n';
}
