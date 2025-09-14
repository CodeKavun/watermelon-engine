#include <string>
#include <fstream>
#include <sstream>
#include "engine_types.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <unordered_map>

class Shader
{
private:
    unsigned int id;
    std::unordered_map<std::string, int> uniformLocations;

    void cacheParameterLocation(std::string name) {
        if (!uniformLocations.contains(name)) {
            uniformLocations.insert({ name, glGetUniformLocation(id, name.c_str()) });
        }
    }
public:
    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);

    inline unsigned int getId() const { return id; }

    void use();
    void clean();

    void setBoolUniform(const std::string& name, const bool& value) {
        cacheParameterLocation(name);
        glUniform1i(uniformLocations[name], value);
    }

    void setIntUniform(const std::string& name, const int& value) {
        cacheParameterLocation(name);
        glUniform1i(uniformLocations[name], value);
    }

    void setFloatUniform(const std::string& name, const float& value) {
        cacheParameterLocation(name);
        glUniform1f(uniformLocations[name], value);
    }

    void setVec2Uniform(const std::string& name, const glm::vec2& value) {
        cacheParameterLocation(name);
        glUniform2fv(uniformLocations[name], 1, glm::value_ptr(value));
    }

    void setVec3Uniform(const std::string& name, const glm::vec3& value) {
        cacheParameterLocation(name);
        glUniform3fv(uniformLocations[name], 1, glm::value_ptr(value));
    }

    void setMat4Uniform(const std::string& name, const glm::mat4& value) {
        cacheParameterLocation(name);
        glUniformMatrix4fv(uniformLocations[name], 1, GL_FALSE, glm::value_ptr(value));
    }

    bool operator==(const Shader& other) const {
        return id == other.getId();
    }
};

class Texture
{
private:
    unsigned int id;
    int width;
    int height;
    int nrChannels;
public:
    Texture() = default;
    Texture(const char* path);
    Texture(unsigned char* data, float width, float height);

    inline unsigned int getId() const { return id; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    void bind();
    void clean();

    bool operator==(const Texture& other) const {
        return id == other.getId();
    }
};

class Camera;

class RenderTarget
{
private:
    unsigned int FBO;
    Texture colorTexture;
    unsigned int depthRBO;

    float width;
    float height;
public:
    RenderTarget(float width, float height);

    inline float getWidth() const { return width; }
    inline float getHeight() const { return height; }

    void use();
    void unuse();
    void clean();

    void draw(Camera& camera, glm::vec2 position, glm::vec2 scale, glm::vec2 origin);
};


class Camera
{
private:
    glm::vec2 position = glm::vec2{ 0.f, 0.f };
    glm::vec2 origin = glm::vec2{ 0.f, 0.f };
    float rotation = 0.f;
    float zoom = 1.f;
    float width, height;
public:
    Camera(float width, float height) : width(width), height(height) {}

    inline glm::vec2 getPosition() const { return -position; }
    inline void setPosition(const glm::vec2& position) { this->position = -position * zoom; }

    inline glm::vec2 getOrigin() const { return origin; }
    inline void setOrigin(const glm::vec2& origin) { this->origin = origin; }

    inline float getRotation() const { return rotation; }
    inline void setRotation(const float& rotation) { this->rotation = rotation; }

    inline float getZoom() const { return zoom; }
    inline void setZoom(const float& zoom) { this->zoom = zoom; }

    inline float getWidth() const { return width; }
    inline void setWidth(const float& width) { this->width = width; }
    
    inline float getHeight() const { return height; }
    inline void setHeight(const float& height) { this->height = height; }

    glm::mat4 getViewMatrix() {
        glm::mat4 view(1.f);
        view = glm::translate(view, glm::vec3{ position + origin, 0.f });
        view = glm::rotate(view, glm::radians(rotation), glm::vec3{ 0.f, 0.f, 1.f });
        view = glm::scale(view, glm::vec3{ zoom, zoom, 1.f });
        return view;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::ortho(0.f, width, height, 0.f, -9999.f, 9999.f);
    }
};


class ObjectDrawer
{
private:
    static unsigned int VAO;
    static unsigned int VBO;
    static unsigned int EBO;
    static Shader defaultShader;

    static Texture currentTexture;
    static Shader currentShader;
public:
    static void initialize();
    static void clean();
    static void bindVertexArray() { glBindVertexArray(VAO); }

    static void clearBackground(Color color);

    static void drawTexture(Camera& camera, Texture& texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, Rectangle* soruce, bool flipH, bool flipV, Shader& shader, float depth = 0);
    static void drawTexture(Camera& camera, Texture& texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, Rectangle* soruce, float depth = 0);
    static void drawTexture(Camera& camera, Texture& texture, glm::vec2 position, glm::vec2 origin, glm::vec2 scale, float rotation, float depth = 0);
    static void drawTexture(Camera& camera, Texture& texture, glm::vec2 position, float depth = 0);
    static void drawTexture(Camera& camera, Texture& texture, glm::vec2 position, Rectangle* soruce, Shader& shader, float depth = 0);
    static void drawTexture(Camera& camera, Texture& texture, glm::vec2 position, Rectangle* soruce, bool flipH, bool flipV, float depth = 0);
};