#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "engine_types.hpp"

class AABB
{
private:
    glm::vec2 position;
    glm::vec2 size;
public:
    AABB() = default;
    AABB(glm::vec2 position, glm::vec2 size) : position(position), size(size) {}

    inline glm::vec2 getPosition() const { return position; }
    inline void setPosition(const glm::vec2& position) { this->position = position; }

    inline glm::vec2 getSize() const { return size; }
    inline void setSize(const glm::vec2& size) { this->size = size; }

    Rectangle getBounds() const {
        return Rectangle{ position.x, position.y, size.x, size.y };
    }

    float getLeft() const { return getBounds().getLeft(); }
    float getRight() const { return getBounds().getRight(); }
    float getTop() const { return getBounds().getTop(); }
    float getBottom() const { return getBounds().getBottom(); }

    bool intersects(const AABB& other) const;

    bool rayVsAABB(const glm::vec2& rayOrigin, const glm::vec2& rayDir, glm::vec2& point, glm::vec2& normal, float& tHitNear);
    bool AABBvsAABB(const AABB& other, glm::vec2& point, glm::vec2& normal, glm::vec2& velocity, float& time, float delta);

    float check(const AABB& other, glm::vec2& normal, glm::vec2& velocity) const;

    AABB getBroadphaseBox(const glm::vec2& velocity) const;
};