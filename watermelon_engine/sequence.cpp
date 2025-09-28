#include "sequence.hpp"
#include <iostream>
#include <fstream>

Animation::Animation(std::string name, std::vector<AnimationFrame*> frames, int frameCount)
{
	this->name = name;
	this->frames = frames;
	this->frameCount = frameCount;
}

AnimationPlayer::AnimationPlayer(std::string animFile)
{
	this->animFile = animFile;

	// Get relative path to a texture
	std::istringstream pathStream(this->animFile);
	std::string subPath;
	std::vector<std::string> subPaths;

	while (std::getline(pathStream, subPath, '/')) {
		subPaths.push_back(subPath);
	}

	subPaths.erase(subPaths.begin());
	subPaths.pop_back();

	std::string textureDir = "";
	for (std::string forlder : subPaths) {
		textureDir += forlder + "/";
	}
	// ------------

	// Read json
	std::ifstream animReader(this->animFile);

	std::string jsonLine;
	std::string jsonString;
	while (animReader >> jsonLine) {
		jsonString += jsonLine;
	}

	nlohmann::ordered_json jsonData = nlohmann::ordered_json::parse(jsonString);

	// Check if it's Aseprite's or my own format
	this->type = jsonData.contains("meta") ? ASEPRITE : ORDINAR;

	switch (this->type)
	{
	case ORDINAR:
	{
		this->texturePath = (std::string)jsonData["texture"];
		this->texture = AssetManager::loadTexture(this->texturePath);

		int regionWidth = jsonData["width"];
		int regionHeight = jsonData["height"];

		TextureAtlas atlas = TextureAtlas::createGrid(texture, regionWidth, regionHeight);
		// printf("%d\n", atlas.getRegionCount());

		for (auto& anim : jsonData["animations"].items()) {
			std::vector<AnimationFrame*> frames;

			for (auto& frame : anim.value()) {
				int regionIndex = frame["index"];
				float delay = frame["delay"];

				AnimationFrame* frameToAdd = new AnimationFrame;
				frameToAdd->regionIndex = regionIndex;
				frameToAdd->delay = delay;
				frameToAdd->source = atlas.getRegion(regionIndex);

				frames.push_back(frameToAdd);
			}

			Animation* animationToAdd = new Animation(anim.key(), frames, frames.size());
			this->animations.insert({ anim.key(), animationToAdd });
			anim.key();
		}

		break;
	}
	case ASEPRITE:
	{
		// Load a texture
		this->texturePath = textureDir + (std::string)jsonData["meta"]["image"];
		this->texture = AssetManager::loadTexture(this->texturePath);

		// Adding aseprite animations
		nlohmann::ordered_json tags = jsonData["meta"]["frameTags"];

		for (auto& tag : tags) {
			// Getting animation datas from Ase
			std::string tagName = (std::string)tag["name"];

			int fromFrame = (int)tag["from"];
			int toFrame = (int)tag["to"];

			//int repeat = (int)tags[i]["repeat"];

			int frameCount = toFrame - fromFrame + 1;

			// TraceLog(LOG_INFO, ("ANIMATION: " + tagName + " -- " + std::to_string(fromFrame) + " -> " + std::to_string(toFrame)).c_str());

			// adding frames
			nlohmann::ordered_json frameArrayData = jsonData["frames"];
			std::vector<AnimationFrame*> frames;

			int frameDataIndex = 0;
			for (auto it = frameArrayData.begin(); it != frameArrayData.end(); it++, frameDataIndex++) {
				if (frameDataIndex < fromFrame) continue;
				if (frameDataIndex > toFrame) break;

				std::string key = it.key();

				float delay = frameArrayData[key]["duration"].get<float>();

				nlohmann::json sourceData = frameArrayData[key]["frame"];
				Rectangle source(
					sourceData["x"].get<float>(),
					sourceData["y"].get<float>(),
					sourceData["w"].get<float>(),
					sourceData["h"].get<float>()
				);

				AnimationFrame* frame = new AnimationFrame;
				frame->regionIndex = frameDataIndex;
				frame->delay = delay * 0.001f;
				frame->source = source;

				frames.push_back(frame);

				// TraceLog(LOG_INFO, ("\t\t Added frame " + std::to_string(frameDataIndex)
				// 	+ " with delay of " + std::to_string(delay * 0.001f) + "s").c_str());
			}

			Animation* anim = new Animation(tagName, frames, frameCount);
			this->animations[tagName] = anim;
		}

		break;
	}
	}
}

void AnimationPlayer::play(std::string name, bool repeat)
{
	if (getCurrentAnimation() != nullptr && getCurrentAnimation()->getName() == name) return;

	if (animations.contains(name)) {
		currentAnimationName = name;
		currentFrameIndex = 0;
		timer = 0;
		playing = true;
	}
}

void AnimationPlayer::stop()
{
	playing = false;
	timer = 0;
}

void AnimationPlayer::update(float delta)
{
	if (getCurrentAnimation() == nullptr || getCurrentFrame() == nullptr || !playing) {
		return;
	}

	timer += delta;
	if (timer >= getCurrentFrame()->delay) {
		timer = 0;

		currentFrameIndex++;
		if (currentFrameIndex >= getCurrentAnimation()->getFrames().size()) currentFrameIndex = 0;
	}
}

void AnimationPlayer::draw(Camera& camera, glm::vec2 position, glm::vec2 scale, glm::vec2 origin, float rotation, Shader* shader, float layerDepth, bool flipX, bool flipY)
{
	if (getCurrentAnimation() == nullptr || getCurrentFrame() == nullptr) {
		ObjectDrawer::drawTexture(camera, texture, position, origin, scale, rotation, nullptr, flipX, flipY, shader, layerDepth);
		return;
	}
	
	Rectangle source = getCurrentFrame()->source;
	ObjectDrawer::drawTexture(camera, texture, position, origin, scale, rotation, &source, flipX, flipY, shader, layerDepth);
}
