#include "world.hpp"

void WorldObject::update(float delta)
{
    globalPosition = position + (parent ? parent->getPosition() : glm::vec2{ 0 });

    for (const auto& addition : additions.getElements()) {
        addition.second.get()->update(delta);
    }

    for (WorldObject* child : children) {
        child->update(delta);
    }
}

void WorldObject::physicsUpdate(float fixedDelta)
{
    for (const auto& addition : additions.getElements()) {
        addition.second.get()->physicsUpdate(fixedDelta);
    }
}

void WorldObject::draw(Camera &camera)
{
    for (WorldObject* child : children) {
        child->draw(camera);
    }
}

void Sprite::update(float delta)
{
    WorldObject::update(delta);
    animPlayer.update(delta);
}

void Sprite::draw(Camera& camera)
{
    glm::vec2 center = centered ? glm::vec2{ animPlayer.getSource().width * scale.x / 2, animPlayer.getSource().height * scale.y / 2 } : glm::vec2{ 0 };
    animPlayer.draw(camera, globalPosition, scale, origin + center, rotation, shader, layerDepth, flipH, flipV);

    WorldObject::draw(camera);
}

void PhysicalBodyAddition::physicsUpdate(float fixedDelta)
{
    glm::vec2 point = { 0, 0 };
    glm::vec2 normal = { 0, 0 };
    float collisionTime = 0.f;

    std::vector<std::pair<float, int>> determinedColliders;
    
    for (int i = 0; i < collidersToCheck.size(); i++) {
        if (collider.AABBvsAABB(collidersToCheck[i], point, normal, velocity, collisionTime, fixedDelta)) {
            determinedColliders.push_back({ collisionTime, i });
        }
    }
    
    std::sort(determinedColliders.begin(), determinedColliders.end());
    
    for (const std::pair<float, int>& dc : determinedColliders) {
        if (collider.AABBvsAABB(collidersToCheck[dc.second], point, normal, velocity, collisionTime, fixedDelta)) {
            float remainingTime = 1.f - collisionTime;
            velocity += normal * glm::abs(velocity) * remainingTime;
        }
    }
    
    owner->setPosition(owner->getPosition() + velocity * fixedDelta);
    collider.setPosition(owner->getPosition());
}
