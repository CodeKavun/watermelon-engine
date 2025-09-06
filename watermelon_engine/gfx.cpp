#include "gfx.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clean()
{
    glDeleteTextures(1, &id);
}


unsigned int ObjectDrawer::VAO;
unsigned int ObjectDrawer::VBO;
unsigned int ObjectDrawer::EBO;
Shader ObjectDrawer::defaultShader;

Texture ObjectDrawer::currentTexture;
Shader ObjectDrawer::currentShader;

void ObjectDrawer::initialize()
{
    defaultShader = Shader("assets/shaders/default.vert", "assets/shaders/default.frag");

    float vertices[] = {
        // positions    // texture coords
        0.f, 0.f, 0.f,  0.f, 1.f,
        1.f, 0.f, 0.f,  1.f, 1.f,
        1.f, 1.f, 0.f,  1.f, 0.f,
        0.f, 1.f, 0.f,  0.f, 0.f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Creating VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Creating VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Creating EBO
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Set Texture vertex attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ObjectDrawer::clean()
{
    defaultShader.clean();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void ObjectDrawer::drawTexture(Camera& camera, Texture &texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, Rectangle* source, bool flipH, bool flipV, Shader& shader, float depth)
{
    if (currentTexture != texture) {
        texture.bind();
        currentTexture = texture;
    }

    if (currentShader != shader) {
        shader.use();
        currentShader = shader;
    }

    shader.setMat4Uniform("projection", camera.getProjectionMatrix());
    shader.setMat4Uniform("view", camera.getViewMatrix());

    // Source of texture
    glm::vec2 sourceSize;
    glm::vec2 uv0;
    glm::vec2 uv1;

    if (source != nullptr) {
        sourceSize = glm::vec2{ source->width, source->height };

        uv0 = glm::vec2{
            source->x / texture.getWidth(),
            1.f - source->height / texture.getHeight()
        };
        uv1 = glm::vec2{
            source->width / texture.getWidth(),
            1.f - source->y / texture.getHeight()
        };
    }
    else {
        sourceSize = glm::vec2{ texture.getWidth(), texture.getHeight() };

        uv0 = glm::vec2{ 0, 0 };
        uv1 = glm::vec2{ 1, 1 };
    }

    // Transformations
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3{ position - origin, depth });
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, glm::vec3{ scale, 1.f });

    // Apply to a shader
    shader.setVec2Uniform("uv0", uv0);
    shader.setVec2Uniform("uv1", uv1);
    shader.setVec2Uniform("uvSize", sourceSize);
    shader.setMat4Uniform("model", model);

    // Draw a texture
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, Rectangle *soruce, float depth)
{
    drawTexture(camera, texture, position, origin, scale, rotation, soruce, false, false, defaultShader, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, float depth)
{
    drawTexture(camera, texture, position, origin, scale, rotation, nullptr, false, false, defaultShader, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, float depth)
{
    drawTexture(camera, texture, position, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 }, 0.f, nullptr, false, false, defaultShader, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, Rectangle *soruce, Shader &shader, float depth)
{
    drawTexture(camera, texture, position, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 }, 0.f, soruce, false, false, shader, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, Rectangle *soruce, bool flipH, bool flipV, float depth)
{
    drawTexture(camera, texture, position, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 }, 0.f, soruce, flipH, flipV, defaultShader, depth);
}
