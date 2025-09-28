#include "world.hpp"

void WorldObject::update(float delta)
{
    globalPosition = position + (parent ? parent->getPosition() : glm::vec2{ 0 });

    for (WorldObject* child : children) {
        child->update(delta);
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
