class Rectangle
{
public:
    float x, y, width, height;

    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

    float getLeft() const { return x; }
    float getRight() const { return x + width; }
    float getTop() const { return y; }
    float getBottom() const { return y + height; }
};

class Color
{
public:
    float r, g, b, a;

    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    Color(float r, float g, float b) : Color::Color(r, g, b, 1.f) {}
};