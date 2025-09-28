#pragma once
#include "sequence.hpp"

class Addition;

class WorldObject
{
protected:
    glm::vec2 position;
    glm::vec2 globalPosition;

    WorldObject* parent = nullptr;
    std::vector<WorldObject*> children;
public:
    WorldObject(glm::vec2 position) : position(position) {}

    inline glm::vec2& getPosition() { return position; }
    inline void setPosition(const glm::vec2 position) { this->position = position; }

    inline glm::vec2& getGlobalPosition() { return globalPosition; }

    inline WorldObject* getParent() { return parent; }
    inline void setParent(WorldObject* parent) { this->parent = parent; }
    
    inline std::vector<WorldObject*>& getChildren() { return children; }
    inline int getChildCount() { return children.size(); }
    inline WorldObject* getChild(int index) { return children[index]; }
    inline void addChild(WorldObject* child) {
        child->setParent(this);
        children.push_back(child);
    }
    inline void removeChild(int index) { children.erase(children.begin() + index); }

    virtual void update(float delta);
    virtual void draw(Camera& camera);
};

class Sprite : public WorldObject
{
private:
    bool visible = true;
    AnimationPlayer animPlayer;

    glm::vec2 origin;
    bool centered = true;
    glm::vec2 scale;
    float rotation;
    float layerDepth = 0;

    bool flipH;
    bool flipV;

    Shader* shader = nullptr;
public:
    Sprite(AnimationPlayer animPlayer, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, bool centered)
        : WorldObject::WorldObject(position), animPlayer(animPlayer), origin(origin), scale(scale), rotation(rotation), centered(centered) {}
    Sprite(AnimationPlayer animPlayer, glm::vec2 position, glm::vec2 scale, float rotation)
        : Sprite::Sprite(animPlayer, position, glm::vec2{ 0 }, scale, rotation, true ) {}
    Sprite(AnimationPlayer animPlayer, glm::vec2 position) : Sprite::Sprite(animPlayer, position, glm::vec2{ 1 }, 0) {}

    inline bool isVisible() { return visible; }
    inline void setVisibility(bool visible) { this->visible = visible; }

    inline AnimationPlayer& getAnimationPlayer() { return animPlayer; }
    inline void setAnimationPlayer(const AnimationPlayer& animPlayer) { this->animPlayer = animPlayer; }

    // Get/Set transforms
    inline glm::vec2& getOrigin() { return origin; }
    inline void setOrigin(const glm::vec2 origin) { this->origin = origin; }

    inline bool isCentered() { return centered; }
    inline void setCentered(bool centered) { this->centered = centered; }

    inline glm::vec2& getScale() { return scale; }
    inline void setScale(const glm::vec2 scale) { this->scale = scale; }

    inline float getRotation() { return rotation; }
    inline void setRotation(float rotation) { this->rotation = rotation; }

    inline float getLayerDepth() { return layerDepth; }
    inline void setLayerDepth(float layerDepth) { this->layerDepth = layerDepth; }

    inline bool isFlippedH() { return flipH; }
    inline void setFlipH(bool flipH) { this->flipH = flipH; }

    inline bool isFlippedV() { return flipV; }
    inline void setFlipV(bool flipV) { this->flipV = flipV; }
    // ---------------

    inline Shader* getShader() { return shader; }
    inline void setShader(Shader* shader) { this->shader = shader; }

    virtual void update(float delta) override;
    virtual void draw(Camera& camera) override;
};

