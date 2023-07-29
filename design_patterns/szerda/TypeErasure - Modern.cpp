#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

class Circle {
	private:
		double radius;
    public:
		explicit Circle(double radius)
			: radius{ radius }
		{}
		double getRadius() const noexcept {
			return radius;
		}
};

void draw(const Circle& circle) {
	std::cout << "Circle: " << circle.getRadius() << '\n';
}

void serialize(const Circle&) {	
}


class Square {
    private:
	    double side;
    public:
	    explicit Square(double side)
		    : side{ side }
	    {}
	    double getSide() const noexcept {
		    return side;
	    }
};

void draw(const Square& square) {
	std::cout << "Square: " << square.getSide() << '\n';
}

void serialize(const Square&) {
}



class Shape {

private:

	struct ShapeConcept {
		virtual ~ShapeConcept() = default;
		virtual void draw() const = 0;
		virtual void serialize() const = 0;
		virtual std::unique_ptr<ShapeConcept> clone() const = 0;
	};

	template<typename GeomShape>
	struct ShapeModel : public ShapeConcept {
		GeomShape shape;
		ShapeModel(const GeomShape& shape)
			: shape{ shape }
		{}
		void draw() const final {
			::draw(shape);
		}
		void serialize() const {
			::serialize(shape);
		}
		std::unique_ptr<ShapeConcept> clone() const final {
			return std::make_unique<ShapeModel>(*this);
		}
	};

	std::unique_ptr<ShapeConcept> pImpl;


	friend void draw(const Shape& shape) {
		shape.pImpl->draw();
	}

	friend void serialize(const Shape& shape) {
		shape.pImpl->serialize();
	}

public:	

	template<typename GeomShape>
	Shape(const GeomShape& gs)
		: pImpl{ new ShapeModel<GeomShape>{gs} }
	{
		
	}

	Shape(const Shape& other) : pImpl{ other.pImpl->clone() }
	{}
	Shape& operator=(const Shape& other) {
		pImpl = other.pImpl->clone();
		return *this;
	}

	Shape(Shape&&) = default;
	Shape& operator=(Shape&&) = default;	
};

void drawAllShapes(const std::vector<Shape>& shapes) {
	for (const auto& shape : shapes)
		draw(shape);
}


int main() {

	std::vector<Shape> shapes;
	
	shapes.push_back(Circle{ 2.0 });
	shapes.push_back(Square{ 1.1 });
	shapes.push_back(Circle{ 4.2 });

	shapes.push_back(4.);

	drawAllShapes(shapes);
}




