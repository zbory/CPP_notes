

struct Foo {

	static constexpr struct Tag1 {} tag1{};
	static constexpr struct Tag2 {} tag2{};

	explicit Foo(Tag1) {}
	explicit Foo(Tag2) {}

};

int main() {

	auto f1 = Foo{ Foo::tag1 };
	auto f2 = Foo{ Foo::tag2 };
}

