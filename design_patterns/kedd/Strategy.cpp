#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

enum class Format{ Markdown, Html };

struct ListStrategy {
	virtual ~ListStrategy() = default;

	virtual void item(std::ostringstream&, const std::string&) = 0;
	virtual void start(std::ostringstream&) {}
	virtual void stop(std::ostringstream&) {}
};

struct MarkdownList : public ListStrategy {
	void item(std::ostringstream &out, const std::string &s) {
		out << " - " << s << '\n';
	}
};

struct HtmlList : public ListStrategy {
	void item(std::ostringstream &out, const std::string &s) {
		out << "  <li>" << s << "</li>\n";
	}
	void start(std::ostringstream &out) {
		out << "<ul>\n";
	}
	void stop(std::ostringstream &out) {
		out << "</ul>\n";
	}
};

class TextProcessor {
    private:
		std::ostringstream out;
		std::unique_ptr<ListStrategy> strategy;

    public:

		void setFormat(Format format) {
			switch (format) {
			    case Format::Markdown:
					strategy = std::make_unique<MarkdownList>();
					break;
				case Format::Html:
					strategy = std::make_unique<HtmlList>();
					break;
			}
		}

		std::string str() { return out.str(); }

		void addList(const std::vector<std::string> &items) {
			strategy->start(out);
			for (const auto &s : items)
				strategy->item(out, s);
			strategy->stop(out);
		}


		void clear() {
			out.str("");
			out.clear();
		}

};



int main() {
	TextProcessor tp;

	tp.setFormat(Format::Markdown);
	tp.addList({"foo", "boo", "baz"});
	std::cout << tp.str() << '\n';


	tp.clear();
	tp.setFormat(Format::Html);
	tp.addList({ "foo", "boo", "baz" });
	std::cout << tp.str() << '\n';
}







