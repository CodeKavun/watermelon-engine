#pragma once
#include <glm/glm.hpp>

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

    bool intersects(const Rectangle& other) {
        float left = std::max(getLeft(), other.getLeft());
        float right = std::min(getRight(), other.getRight());
        float top = std::max(getTop(), other.getTop());
        float bottom = std::min(getBottom(), other.getBottom());
        return left < right && top < bottom;
    }
};

class Color
{
public:
    float r, g, b, a;

    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    Color(float r, float g, float b) : Color::Color(r, g, b, 1.f) {}

    glm::vec4 toVec4() {
        return glm::vec4{ r, g, b, a };
    }
};