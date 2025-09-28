#include "gfx.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "core.hpp"

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

Texture::Texture(unsigned char *data, float width, float height)
{
    this->width = width;
    this->height = height;
    nrChannels = 4;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clean()
{
    glDeleteTextures(1, &id);
}

RenderTarget::RenderTarget(float width, float height)
    : width(width), height(height)
{
    // Generate framebuffer
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create color texture
    colorTexture = Texture(nullptr, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.getId(), 0);

    // Create depth buffer
    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);
}

void RenderTarget::unuse()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
}

void RenderTarget::clean()
{
    glDeleteFramebuffers(1, &FBO);
    colorTexture.clean();
    glDeleteRenderbuffers(1, &depthRBO);
}

void RenderTarget::draw(Camera &camera, glm::vec2 position, glm::vec2 scale, glm::vec2 origin)
{
    ObjectDrawer::drawTexture(camera, colorTexture, position, origin, scale, 0.f);
}

unsigned int ObjectDrawer::VAO;
unsigned int ObjectDrawer::VBO;
unsigned int ObjectDrawer::EBO;

Shader ObjectDrawer::defaultShader;
Shader ObjectDrawer::solidColorShader;
Shader ObjectDrawer::circleShader;

Texture ObjectDrawer::currentTexture;
Shader* ObjectDrawer::currentShader;

void ObjectDrawer::initialize()
{
    defaultShader = Shader("assets/shaders/default.vert", "assets/shaders/default.frag");
    solidColorShader = Shader("assets/shaders/shapes/shape.vert", "assets/shaders/shapes/shape.frag");
    circleShader = Shader("assets/shaders/shapes/shape.vert", "assets/shaders/shapes/circle.frag");
    currentShader = &defaultShader;

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

void ObjectDrawer::clearBackground(Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, Rectangle *source, bool flipH, bool flipV, Shader *shader, float depth)
{
    if (currentTexture != texture) {
        texture.bind();
        currentTexture = texture;
    }

    if (currentShader != shader) {
        if (!shader) {
            currentShader = &defaultShader;
            currentShader->use();
        }
        else {
            currentShader = shader;
            currentShader->use();
        }
    }

    currentShader->setMat4Uniform("projection", camera.getProjectionMatrix());
    currentShader->setMat4Uniform("view", camera.getViewMatrix());

    // Source of texture
    glm::vec2 sourceSize;
    glm::vec2 uv0;
    glm::vec2 uv1;

    if (source != nullptr) {
        sourceSize = glm::vec2{ source->width, source->height };

        uv0 = glm::vec2{
            source->getLeft() / texture.getWidth(),
            1.f - source->getBottom() / texture.getHeight()
        };
        uv1 = glm::vec2{
            source->getRight() / texture.getWidth(),
            1.f - source->getTop() / texture.getHeight()
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
    currentShader->setVec2Uniform("uv0", uv0);
    currentShader->setVec2Uniform("uv1", uv1);
    currentShader->setVec2Uniform("uvSize", sourceSize);
    currentShader->setVec4Uniform("color", glm::vec4{ 1, 1, 1, 1 });
    currentShader->setMat4Uniform("model", model);

    // Draw a texture
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, Rectangle *soruce, float depth)
{
    drawTexture(camera, texture, position, origin, scale, rotation, soruce, false, false, nullptr, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, float depth)
{
    drawTexture(camera, texture, position, origin, scale, rotation, nullptr, false, false, nullptr, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, float depth)
{
    drawTexture(camera, texture, position, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 }, 0.f, nullptr, false, false, nullptr, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, Rectangle *soruce, Shader *shader, float depth)
{
    drawTexture(camera, texture, position, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 }, 0.f, soruce, false, false, shader, depth);
}

void ObjectDrawer::drawTexture(Camera &camera, Texture &texture, glm::vec2 position, Rectangle *soruce, bool flipH, bool flipV, float depth)
{
    drawTexture(camera, texture, position, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 }, 0.f, soruce, flipH, flipV, nullptr, depth);
}

void ObjectDrawer::drawRectangle(Camera &camera, Rectangle rectangle, Color color, float layerDepth)
{
    if (currentShader != &solidColorShader) {
        currentShader = &solidColorShader;
        currentShader->use();
    }

    currentShader->setMat4Uniform("projection", camera.getProjectionMatrix());
    currentShader->setMat4Uniform("view", camera.getViewMatrix());
    
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3{ rectangle.x, rectangle.y, layerDepth });
    model = glm::scale(model, glm::vec3{ rectangle.width, rectangle.height, 1.f });

    currentShader->setMat4Uniform("model", model);
    currentShader->setVec4Uniform("color", color.toVec4());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ObjectDrawer::drawCircle(Camera &camera, glm::vec2 position, float radius, Color color, float layerDepth)
{
    if (currentShader != &circleShader) {
        currentShader = &circleShader;
        currentShader->use();
    }

    currentShader->setMat4Uniform("projection", camera.getProjectionMatrix());
    currentShader->setMat4Uniform("view", camera.getViewMatrix());
    
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3{ position - radius / 2, layerDepth });
    model = glm::scale(model, glm::vec3{ radius, radius, 1.f });

    currentShader->setMat4Uniform("model", model);
    currentShader->setVec4Uniform("color", color.toVec4());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

TextureAtlas TextureAtlas::createGrid(Texture texture, int cellWidth, int cellHeight)
{
    int columns = (int)texture.getWidth() / cellWidth;
    int rows = (int)texture.getHeight() / cellHeight;
    int count = columns * rows;

    TextureAtlas atlas(texture);

    for (int i = 0; i < count; i++) {
        Rectangle source = Rectangle{
            (float)(i % columns * cellWidth),
            (float)(i / columns * cellHeight),
            (float)cellWidth,
            (float)cellHeight
        };

        atlas.addRegion(source);
    }

    return atlas;
}

void TextureAtlas::addRegion(Rectangle source)
{
    regions.push_back(source);
}

void TextureAtlas::addRegion(float x, float y, float width, float height)
{
    regions.push_back(Rectangle{ x, y, width, height });
}

void TextureAtlas::removeRegion(int index)
{
    regions.erase(regions.begin() + index);
}
