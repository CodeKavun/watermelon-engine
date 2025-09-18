#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "utils.hpp"

enum AnimationType
{
	ORDINAR,
	ASEPRITE
};

struct AnimationFrame
{
	int regionIndex;
	float delay;
	Rectangle source;
};

class Animation
{
public:
	Animation() {}
	Animation(std::string name, std::vector<AnimationFrame*> frames, int frameCount);

	inline std::string getName() const { return name; }

	inline std::vector<AnimationFrame*> getFrames() { return frames; }
	inline AnimationFrame* getFrame(int index) { return frames[index]; }
	inline int getFrameCount() const { return frameCount; }

private:
	std::string name;
	std::vector<AnimationFrame*> frames;
	int frameCount;
	int* reapeat;
};

class AnimationPlayer
{
public:
	AnimationPlayer() {}
	AnimationPlayer(std::string animFile);

	inline std::string getAnimationPath() const { return animFile; }
	inline std::string getTexturePath() const { return texturePath; }

	inline std::unordered_map<std::string, Animation*> getAnimations() { return animations; }

	inline Texture getTexture() const { return texture; }

	inline AnimationType getType() const { return type; }

	inline bool isPlaying() const { return playing; }

	inline Animation* getCurrentAnimation() { return animations[currentAnimationName]; }
	inline AnimationFrame* getCurrentFrame() { return getCurrentAnimation()->getFrame(currentFrameIndex); }

	inline Rectangle getSource() {
		Rectangle defaultRect = { 0, 0, texture.getWidth(), texture.getHeight() };
		return getCurrentAnimation() != nullptr && getCurrentFrame() != nullptr
			? getCurrentFrame()->source : defaultRect;
	}

	void play(std::string name, bool repeat);
	void stop();
	void update(float delta);
	void draw(Camera& camera, glm::vec2 position, glm::vec2 scale, glm::vec2 origin, float rotation, Shader& shader, float layerDepth = 0, bool flipX = false, bool flipY = false);

private:
	std::string animFile;
	std::string texturePath;
	std::unordered_map<std::string, Animation*> animations;
	AnimationType type;

	Texture texture;
	bool playing = false;
	float timer = 0;

	std::string currentAnimationName;
	int currentFrameIndex = 0;
};
