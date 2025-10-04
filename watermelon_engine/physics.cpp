#include "physics.hpp"

bool AABB::intersects(const AABB &other) const
{
    return getBounds().intersects(other.getBounds());
}

bool AABB::rayVsAABB(const glm::vec2 &rayOrigin, const glm::vec2 &rayDir, glm::vec2 &point, glm::vec2 &normal, float &tHitNear)
{
    glm::vec2 tNear = (position - rayOrigin) / rayDir;
    glm::vec2 tFar = (position + size - rayOrigin) / rayDir;

    if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
    if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

    if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

    tHitNear = std::max(tNear.x, tNear.y);
    float tHitFar = std::min(tFar.x, tFar.y);

    if (tHitFar < 0) return false;

    point = rayOrigin + tHitNear * rayDir;

    if (tNear.x > tNear.y) {
        normal = glm::vec2{ rayDir.x < 0 ? 1 : -1, 0 };
    }
    else if (tNear.x < tNear.y) {
        normal = glm::vec2{ 0, rayDir.y < 0 ? 1 : -1 };
    }

    return true;
}

bool AABB::AABBvsAABB(const AABB &other, glm::vec2 &point, glm::vec2 &normal, glm::vec2 &velocity, float &time, float delta)
{
    if (velocity.x == 0.f && velocity.y == 0.f) return false;

    AABB expandedAABB(other.getPosition() - size / 2.f, other.getSize() + size);
    if (expandedAABB.rayVsAABB(position + size / 2.f, velocity * delta, point, normal, time)) {
        return time >= -0.001f && time < 1.f;
    }

    return false;
}

float AABB::check(const AABB &other, glm::vec2 &normal, glm::vec2 &velocity)
{
    float xInvEntry, yInvEntry, xInvExit, yInvExit;

        // Find distance between objects on the near and far sides
        if (velocity.x > 0.f) {
            xInvEntry = other.getLeft() - getRight();
            xInvExit = other.getRight() - getLeft();
        }
        else {
            xInvEntry = other.getRight() - getLeft();
            xInvExit = other.getLeft() - getRight();
        }

        if (velocity.y > 0.f) {
            yInvEntry = other.getTop() - getBottom();
            yInvExit = other.getBottom() - getTop();
        }
        else {
            yInvEntry = other.getBottom() - getTop();
            yInvExit = other.getTop() - getBottom();
        }

        // Find collision and leaving times
        float xEntry, yEntry, xExit, yExit;

        if (velocity.x == 0.f) {
            xEntry = -std::numeric_limits<float>::infinity();
            xExit = std::numeric_limits<float>::infinity();
        }
        else {
            xEntry = xInvEntry / velocity.x;
            xExit = xInvExit / velocity.x;
        }

        if (velocity.y == 0.f) {
            yEntry = -std::numeric_limits<float>::infinity();
            yExit = std::numeric_limits<float>::infinity();
        }
        else {
            yEntry = yInvEntry / velocity.y;
            yExit = yInvExit / velocity.y;
        }

        // Find the earliest and latest collision times
        float entryTime = std::max(xEntry, yEntry);
        float exitTime = std::min(xExit, yExit);

        // Test if there's no collision
        if (entryTime > exitTime || (xEntry < 0.f && yEntry < 0.f) || xEntry > 1.f || yEntry > 1.f) {
            normal = glm::vec2{ 0.f, 0.f };
            return 1.f;
        }
        else {
            if (xEntry > yEntry) {
                normal = glm::vec2{ xInvEntry < 0.f ? 1.f : -1.f, 0.f };
            }
            else {
                normal = glm::vec2{ 0.f, yInvEntry < 0.f ? 1.f : -1.f };
            }

            return entryTime;
        }
}

AABB AABB::getBroadphaseBox(const glm::vec2& velocity) const
{
    AABB broadphaseBox(
        glm::vec2{
            velocity.x > 0.f ? position.x : position.x + velocity.x,
            velocity.y > 0.f ? position.y : position.y + velocity.y
        },
        glm::vec2{
            velocity.x > 0.f ? velocity.x + size.x : size.x - velocity.x,
            velocity.y > 0.f ? velocity.y + size.y : size.y - velocity.y
        }
    );
    return broadphaseBox;
}
