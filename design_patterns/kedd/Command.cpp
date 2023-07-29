#include <iostream>
#include <vector>
#include <memory>

class Number;

class Memento {
    private: 
        int value;
    public:
        Memento(int value) : value{ value } {}

       friend Number;
};

class Number {
    private:
        int value;
    public:
        explicit Number(int value) : value{ value }{}
        int getValue() const noexcept { return value; }

        void div2() { value /= 2; }
        void mul2() { value *= 2; }

        auto save() const {
            return std::make_unique<Memento>(value);
        }

        void load(const Memento *m) { value = m->value; }
};

struct Command {
    virtual ~Command() = default;

    virtual void doit() = 0;
    virtual void undo() = 0;
};


class Mul2Command : public Command {
    private:
        Number &num;
    public:
        explicit Mul2Command(Number &num) : num{ num } {}
        void doit() final {
            num.mul2();
        }
        void undo() final {
            num.div2();
        }
};

class Div2Command : public Command {
    private:
        Number &num;
        std::unique_ptr<Memento> mem;
    public:
        explicit Div2Command(Number &num) : num{ num } {}
        void doit() final {
            mem = num.save();
            num.div2();
        }
        void undo() final {
            if (mem) {
                num.load(mem.get());
                mem.release();
            }
        }
};

int main() {
    int i;
    std::cout << "Integer: ";
    std::cin >> i;

    Number num{ i };

    std::vector<std::unique_ptr<Command>> ready;    

    std::cout << "Exit[0], Mul2[1], Div2[2], Undo[3]: ";
    std::cin >> i;
    while (i) {
        switch (i) {
            case 1:
                {
                    auto cmd = std::make_unique<Mul2Command>(num);
                    cmd->doit();
                    ready.push_back(std::move(cmd));
                }
                break;
            case 2:
                {
                    auto cmd = std::make_unique<Div2Command>(num);
                    cmd->doit();
                    ready.push_back(std::move(cmd));
                }
                break;
            case 3:
                {
                    if (ready.empty())
                        std::cout << "*** Cannot undo.... ***\n";
                    else {
                        ready.back()->undo();
                        ready.pop_back();
                    }
                    break;
                }
        }

        std::cout << '\t' << num.getValue() << '\n';
        std::cout << "Exit[0], Mul2[1], Div2[2], Undo[3]: ";
        std::cin >> i;
    }


}
