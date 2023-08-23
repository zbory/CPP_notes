//////////////////////////////////////////////////////////////////////////
// Training example

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

//////////////////////////////////////////////////////////////////////////
// (Default) Constructors with Tag dispatch

class MyClass
{
public:
    static struct ConstructThisWay{} constructThisWay;
    static struct ConstructThatWay{} constructThatWay;

    explicit MyClass(ConstructThisWay);
    explicit MyClass(ConstructThatWay);
};


MyClass x(MyClass::constructThatWay);

//////////////////////////////////////////////////////////////////////////
//              Tag Dispatch VS Enum

// (1) Structure of the code
// Tag Dispatch: separation of logic with same signature functions, good if the logic is quite different

struct BehaveThisWay{};
struct BehaveThatWay{};

void f(int argument, BehaveThisWay);
{
    // use argument this way
}

void f(int argument, BehaveThatWay);
{
    // use argument that way
}

// Enum: logic in single function, good if the logic is mostly the same and then test the enum at the few junction points

enum class BehaviourType
{
    thisWay,
    thatWay
};

void f(int argument, BehaviourType behaviourType);
{
    // check the value of behaviourType and behave accordingly
}

// (2) Time of resolution
// Tag Dispatch: compile time
// Enum: runtime

if (myBehaviour == BehaviourType::thisWay)
{
    f(value, BehaveThisWay());
}
else if (myBehaviour == BehaviourType::thatWay)
{
    f(value, BehaveThatWay());
}