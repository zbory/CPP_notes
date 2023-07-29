#include <iostream>
#include <string>
#include <memory>
#include <map>

class Variable;

class Context {
    private:
        std::map<std::string, bool> vars;
    public:
        void assign(const Variable*, bool);
        bool lookup(const Variable*) const;
};

struct BooleanExpression {
    virtual ~BooleanExpression() = default;
    virtual bool evaluate(const Context&) const = 0;
};

class Variable : public BooleanExpression {
    private:
        std::string name;
    public:
        explicit Variable(const std::string &name)
            : name{ name }
        {}

        bool evaluate(const Context &ctx) const final {
            return ctx.lookup(this);
        }

        const std::string& getName() const noexcept {
            return name;
        }
};

void Context::assign(const Variable *var, bool val) {
    vars[var->getName()] = val;
}

bool Context::lookup(const Variable *var) const {
    const auto it = vars.find(var->getName());
    if (it != std::cend(vars))
        return it->second;
    return false;
}

class Constant : public BooleanExpression {
    private:
        bool value;
    public:
        Constant(bool value) : value{ value }{}
        bool evaluate(const Context&) const final {
            return value;
        }
};

class OrExpression : public BooleanExpression {
    private:
        std::shared_ptr<BooleanExpression> lhs;
        std::shared_ptr<BooleanExpression> rhs;
    public:
        OrExpression(const std::shared_ptr<BooleanExpression> &lhs,
                     const std::shared_ptr<BooleanExpression> &rhs)
            : lhs{ lhs }, rhs{ rhs } {}

        bool evaluate(const Context &ctx) const final {
            return lhs->evaluate(ctx) || rhs->evaluate(ctx);
        }
};

class AndExpression : public BooleanExpression {
    private:
        std::shared_ptr<BooleanExpression> lhs;
        std::shared_ptr<BooleanExpression> rhs;
    public:
        AndExpression(const std::shared_ptr<BooleanExpression> &lhs,
                      const std::shared_ptr<BooleanExpression> &rhs)
            : lhs{ lhs }, rhs{ rhs } {}

        bool evaluate(const Context &ctx) const final {
            return lhs->evaluate(ctx) && rhs->evaluate(ctx);
        }
};

class NotExpression : public BooleanExpression {
    private:
        std::shared_ptr<BooleanExpression> op;
    public:
        NotExpression(const std::shared_ptr<BooleanExpression> &op)
            : op{ op } {}

        bool evaluate(const Context &ctx) const final {
            return !op->evaluate(ctx);
        }
};


int main() {

    Context ctx;
    auto x = std::make_shared<Variable>("x");
    auto y = std::make_shared<Variable>("y");
    auto z = std::make_shared<Variable>("z");

    auto e = std::make_shared<OrExpression>(
                 std::make_shared<AndExpression>(std::make_shared<Constant>(true), x),
                 std::make_shared<AndExpression>(y, std::make_shared<NotExpression>(z))
             );

    ctx.assign(x.get(), true);
    ctx.assign(y.get(), false);
    ctx.assign(z.get(), true);

    bool result = e->evaluate(ctx);
    std::cout << std::boolalpha << "Result: " << result << '\n';

}





