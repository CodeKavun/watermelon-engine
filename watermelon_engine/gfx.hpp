#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
private:
    unsigned int id;
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    inline unsigned int& getId() { return id; }

    void use();
    void clean();
};

class Texture
{
private:
    unsigned int id;
    int width;
    int height;
    int nrChannels;
public:
    Texture(const char* path);

    inline unsigned int& getId() { return id; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    void clean();
};

class Camera
{
private:
    glm::vec2 position = glm::vec2{ 0.f, 0.f };
    float zoom = 1.f;
    float rotation = 0.f;
    float width, height;
public:
    Camera(float width, float height) : width(width), height(height) {}

    inline glm::vec2 getPosition() const { return position; }
    inline void setPosition(const glm::vec2& position) { this->position = position; }

    inline float getZoom() const { return zoom; }
    inline void setZoom(const float& zoom) { this->zoom = zoom; }

    inline float getRotation() const { return rotation; }
    inline void setRotation(const float& rotation) { this->rotation = rotation; }

    inline float getWidth() const { return width; }
    inline void setWidth(const float& width) { this->width = width; }
    
    inline float getHeight() const { return height; }
    inline void setHeight(const float& height) { this->height = height; }

    glm::mat4 getViewMatrix() {
        glm::mat4 view = glm::mat4(1.f);
        view = glm::translate(view, glm::vec3{ -position, 0.f });
        view = glm::rotate(view, glm::radians(-rotation), glm::vec3{ 0.f, 0.f, 1.f });
        view = glm::scale(view, glm::vec3{ zoom, zoom, 1.f });
        return view;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::ortho(0.f, width, height, 0.f, -100.f, 100.f);
    }
};