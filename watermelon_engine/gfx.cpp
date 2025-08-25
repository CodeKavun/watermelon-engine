#include "gfx.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/gl.h>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.open(vertexPath);
    fragFile.open(fragmentPath);

    std::stringstream vertStream, fragStream;
    vertStream << vertFile.rdbuf();
    fragStream << fragFile.rdbuf();

    vertFile.close();
    fragFile.close();

    vertexShaderCode = vertStream.str();
    fragmentShaderCode = fragStream.str();
    const char* cVertShaderCode = vertexShaderCode.c_str();
    const char* cFragmentShaderCode = fragmentShaderCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &cVertShaderCode, nullptr);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &cFragmentShaderCode, nullptr);
    glCompileShader(fragment);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::clean()
{
    glDeleteProgram(id);
}

Texture::Texture(const char *path)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}

void Texture::clean()
{
    glDeleteTextures(1, &id);
}
