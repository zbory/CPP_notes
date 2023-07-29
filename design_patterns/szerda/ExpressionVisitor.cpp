#include <iostream>
#include <memory>
#include <string>

struct AddExpression;
struct MulExpression;
class NumExpression;

struct ExpressionVisitor {
	virtual ~ExpressionVisitor() = default;

	virtual std::string visit(const AddExpression&) const = 0;
	virtual std::string visit(const MulExpression&) const = 0;
	virtual std::string visit(const NumExpression&) const = 0;
};

struct Expression {
	virtual ~Expression() = default;
	virtual std::string accept(const ExpressionVisitor&) const = 0;
};

using ExpressionPtr = std::unique_ptr<Expression>;

template<typename T>
class BinaryExpression : public Expression {
    private:
		ExpressionPtr lhs;
		ExpressionPtr rhs;
    public:
		BinaryExpression(ExpressionPtr lhs, ExpressionPtr rhs)
			: lhs{ std::move(lhs) }
			, rhs{ std::move(rhs) }
		{}

		const Expression& left() const { return *lhs; }
		const Expression& right() const { return *rhs; }

		std::string accept(const ExpressionVisitor &visitor) const final {
			const T& child = static_cast<const T&>(*this);
			return visitor.visit(child);
		}
};


struct AddExpression : public BinaryExpression<AddExpression> {
	using BinaryExpression::BinaryExpression;
};

struct MulExpression : public BinaryExpression<MulExpression> {
	using BinaryExpression::BinaryExpression;
};

class NumExpression : public Expression {
    private:
		double value;
    public:
		NumExpression(double value)
			: value{ value }
		{}
		auto getValue() const noexcept { return value; }

		std::string accept(const ExpressionVisitor& visitor) const final {
			return visitor.visit(*this);
		}
};


template<typename S>
class ExpressionPrinter : public ExpressionVisitor {
    private:
		std::ostream &out;
    public:
		ExpressionPrinter(std::ostream &out)
			: out{ out }
		{}

		void print(const Expression& exp) {
			out << exp.accept(*this);
		}

		std::string visit(const AddExpression &expr) const final {
			return S::print(
				"+",
				expr.left().accept(*this),
				expr.right().accept(*this)
			);
		}

		std::string visit(const MulExpression &expr) const final {
			return S::print(
				"*",
				expr.left().accept(*this),
				expr.right().accept(*this)
			);
		}

		std::string visit(const NumExpression &expr) const final {
			return std::to_string(expr.getValue());
		}
};

struct PrefixStrategy {
	static std::string print(const std::string& op,
		                     const std::string& lhs,
		                     const std::string& rhs) {
		return op + " " + lhs + " " + rhs;
	}
};

struct PostfixStrategy {
	static std::string print(const std::string& op,
		                     const std::string& lhs,
		                     const std::string& rhs) {
		return lhs + " " + rhs + " " + op;
	}
};

struct InfixStrategy {
	static std::string print(const std::string& op,
		                     const std::string& lhs,
		                     const std::string& rhs) {
		return "(" + lhs + " " + op + " " + rhs + ")";
	}
};


int main() {

	auto expr = std::make_unique<AddExpression>(
		std::make_unique<NumExpression>(42),
		std::make_unique<MulExpression>(
			std::make_unique<NumExpression>(3),
			std::make_unique<NumExpression>(4)
		)
	);

	std::cout << "\nInfix:\n";
	{
		ExpressionPrinter<InfixStrategy> ep{ std::cout };
		ep.print(*expr);
	}

	std::cout << "\nPostfix:\n";
	{
		ExpressionPrinter<PostfixStrategy> ep{ std::cout };
		ep.print(*expr);
	}
}
