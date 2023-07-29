#include <iostream>
#include <vector>

class Number;

class Memento {
    private:
        int state;
    public:
        Memento(int val) : state{ val } {}

    friend class Number;
};

class Number {
    private:
        int value;
    public:
        explicit Number(int i) : value{ i } {}

        void div2() { value /= 2; }
        void mul2() { value *= 2; }

        int getValue() const noexcept { return  value; }

        Memento createMemento() const {
            return Memento{ value };
        }

        void load(const Memento &m) { value = m.state; }
};

class Commander {

    public:

        typedef void(Number::*Action)();

    private:

        Number *object;
        std::vector<Action> actions;
        std::vector<Memento> mementos;

    public:

        Commander(Number *object) : object{ object } {}
        void addAction(const Action &a) {
            actions.push_back(a);
        }
        void execute(unsigned u) {
            if (u == 0 || u > actions.size())
                return;
            u--;
            mementos.push_back(object->createMemento());
            (object->*actions[u])();
        }

        void undo() {
            if (mementos.empty()) {
                std::cout << "*** Attempt to run off the end! ***\n";
                return;
            }

            Memento p = mementos.back();
            mementos.pop_back();
            object->load(p);
        }
};

int main() {

    int i;
    std::cout << "Integer: ";
    std::cin >> i;

    Number num{ i };
    Commander cmd{ &num };
    cmd.addAction(&Number::mul2);
    cmd.addAction(&Number::div2);

    std::cout << "Exit[0], Mul2[1], Div[2], Undo[3]: ";
    std::cin >> i;
    while (i) {
        if (i == 3)
            cmd.undo();
        else
            cmd.execute(i);

        std::cout << "\t" << num.getValue() << '\n';
        std::cout << "Exit[0], Mul2[1], Div[2], Undo[3]: ";
        std::cin >> i;
    }
}
