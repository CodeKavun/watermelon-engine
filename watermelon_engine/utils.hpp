#pragma once
#include "gfx.hpp"
#include <unordered_map>

enum ShaderLoadType {
    SHADER_LOAD_TYPE_VERT,
    SHADER_LOAD_TYPE_FRAG
};

class AssetManager
{
private:
    static std::unordered_map<std::string, Texture> cachedTextures;
    static std::unordered_map<std::string, Shader> cachedShaders;
public:
    static Texture& loadTexture(std::string path);
    static Shader& loadShader(std::string vertFilePath, std::string fragFilePath);
    static Shader& loadShader(std::string filePath, ShaderLoadType loadType);

    static void cleanAll();
};


float normalizeAxis(float axis, float maxValue);