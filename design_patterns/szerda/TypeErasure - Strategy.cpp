#include <iostream>
#include <memory>
#include <vector>

class Shape {
    public:
        virtual ~Shape() = default;

        virtual void draw() const = 0;
        virtual void serialize() const = 0;
};

class Circle;

class DrawCircleStrategy {
    public:
        virtual ~DrawCircleStrategy() = default;
        virtual void draw(const Circle&) const = 0;
};

class Circle : public Shape {
    private:
        double radius;
        std::unique_ptr<DrawCircleStrategy> drawing;
    public:
        Circle(double radius, std::unique_ptr<DrawCircleStrategy> drawing)
            : radius{ radius }
            , drawing{ std::move(drawing) }
        {}
        double getRadius() const noexcept {
            return radius;
        }
        void draw() const override {
            drawing->draw(*this);
        }
        void serialize() const override {}
};

class Square;

class DrawSquareStrategy {
    public:
        virtual ~DrawSquareStrategy() = default;
        virtual void draw(const Square&) const = 0;
};

class Square : public Shape {
    private:
         double side;
         std::unique_ptr<DrawSquareStrategy> drawing;
    public:
        Square(double side, std::unique_ptr<DrawSquareStrategy> drawing)
            : side{ side }
            , drawing{ std::move(drawing) }
        {}
        double getSide() const noexcept {
            return side;
        }
        void draw() const override {
            drawing->draw(*this);
        }
        void serialize() const override {}
};

class TestCircleStrategy : public DrawCircleStrategy {
    private:
        std::ostream& out;
    public:
        TestCircleStrategy(std::ostream& out)
            : out{ out }
        {}
        void draw(const Circle& circle) const final {
            out << "circle: " << circle.getRadius() << '\n';
        }
};

class TestSquareStrategy : public DrawSquareStrategy {
    private:
        std::ostream& out;
    public:
        TestSquareStrategy(std::ostream& out)
            : out{ out }
        {}
        void draw(const Square& square) const final {
            out << "square: " << square.getSide() << '\n';
        }
};


void drawAllShapes(const std::vector<std::unique_ptr<Shape>>& shapes) {
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

int main() {
    using Shapes = std::vector<std::unique_ptr<Shape>>;
   

    Shapes shapes;
    shapes.push_back(
        std::make_unique<Circle>(2.0,
             std::make_unique<TestCircleStrategy>(std::cout)
        )
    );
    shapes.push_back(
        std::make_unique<Square>(1.5,
            std::make_unique<TestSquareStrategy>(std::cout)
            )
    );
    shapes.push_back(
        std::make_unique<Circle>(4.2,
            std::make_unique<TestCircleStrategy>(std::cout)
            )
    );

    drawAllShapes(shapes);
}


