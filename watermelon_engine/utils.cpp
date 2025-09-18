#include "utils.hpp"

float normalizeAxis(float axis, float maxValue)
{
    return axis / (axis < 0 ? maxValue + 1 : maxValue);
}


std::unordered_map<std::string, Texture> AssetManager::cachedTextures;
std::unordered_map<std::string, Shader> AssetManager::cachedShaders;

Texture &AssetManager::loadTexture(std::string path)
{
    if (!cachedTextures.contains(path)) {
        Texture texture(("assets/sprites/" + path).c_str());
        cachedTextures[path] = texture;
    }
    
    return cachedTextures[path];
}

Shader &AssetManager::loadShader(std::string vertFilePath, std::string fragFilePath)
{
    std::string assetKey = vertFilePath + ";" + fragFilePath;

    if (!cachedShaders.contains(assetKey)) {
        Shader shader(("assets/shaders/" + vertFilePath).c_str(), ("assets/shaders/" + fragFilePath).c_str());
        cachedShaders[assetKey] = shader;
    }

    return cachedShaders[assetKey];
}

Shader &AssetManager::loadShader(std::string filePath, ShaderLoadType loadType)
{
    if (!cachedShaders.contains(filePath)) {
        Shader shader;
        switch (loadType) {
            case SHADER_LOAD_TYPE_VERT: {
                shader = Shader(("assets/shaders/" + filePath).c_str(), "assets/shaders/default.frag");
                break;
            }
            case SHADER_LOAD_TYPE_FRAG: {
                shader = Shader("assets/shaders/default.vert", ("assets/shaders/" + filePath).c_str());
                break;
            }
        }
        cachedShaders[filePath] = shader;
    }

    return cachedShaders[filePath];
}

void AssetManager::cleanAll()
{
    for (auto& texture : cachedTextures) {
        texture.second.clean();
    }
    cachedTextures.clear();

    for (auto& shader : cachedShaders) {
        shader.second.clean();
    }
    cachedShaders.clear();
}
