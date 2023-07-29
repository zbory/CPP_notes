#include <iostream>
#include <string>
#include <map>

struct Request {
	std::string name;
	std::string password;

	std::string recourse;
	std::string payload;
};

struct Middleware {
	Middleware *next = nullptr;

	virtual ~Middleware() {
		delete next;
	}

	virtual bool handle(const Request&) const = 0;
	void setNext(Middleware *n) {
		if (next)
			delete next;
		next = n;
	}
};

struct Authentication : public Middleware {
    private:
		const std::map<std::string, std::string>& getUserDB() const {
			static const std::map<std::string, std::string> userDB{
				{"Willy", "12345"},
				{"Wonka", "csoki"}
			};
			return userDB;
		}
		bool isValid(const std::string &name, const std::string &passwd) const {
			const auto it = getUserDB().find(name);
			if (it != getUserDB().cend()) {
				return it->second == passwd;
			}
			return false;
		}
    public:
		Authentication() = default;
		bool handle(const Request &req) const final {
			if (!isValid(req.name, req.password))
				return false;
			else if (next)
				return next->handle(req);
			return true;
		}
};


int main() {
	Request req{ "Willy", "querty", "index.html", "almafa=kortefa" };

	Middleware *login = new Authentication{};
	login->setNext(new Authentication{});

	std::cout << "res: " << login->handle(req) << '\n';

	delete login;
}

