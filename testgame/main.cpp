#include <watermelon_engine/core.hpp>
#include <watermelon_engine/gfx.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
    // positions    // texture coords
    0.f, 0.f, 0.f,  0.f, 1.f,
    1.f, 0.f, 0.f,  1.f, 1.f,
    1.f, 1.f, 0.f,  1.f, 0.f,
    0.f, 1.f, 0.f,  0.f, 0.f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f), 
    glm::vec3( 20.f,  50.f, 0.f), 
    glm::vec3(150.f, 220.f, 0.f),  
    glm::vec3(380.f, 200.f, 0.f),  
    glm::vec3(240.f, 40.f, 0.f),  
    glm::vec3(1100.f,  30.f, -900.f),  
    glm::vec3(1150.f, 50.f, -200.f),  
    glm::vec3(1000.f,  200.f, 0.f), 
    glm::vec3(150.f,  200.f, 0.f), 
    glm::vec3(130.f,  160.f, 0.f)  
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

int main(int argc, char** argv) {
    Engine::initialize("Some silly test", 1280, 720);

    // Tech shit for rendering;
    // Creating VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Creating VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Creating EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Set Texture vertex attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // -----------------------------

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // -----------------------

    Shader defaultShader("assets/shaders/default.vert", "assets/shaders/default.frag");
    Texture texture("assets/sprites/MainMenuArt.png");

    Camera camera(1280, 720);

    unsigned int modelMatLoc = glGetUniformLocation(defaultShader.getId(), "model");
    unsigned int viewMatLoc = glGetUniformLocation(defaultShader.getId(), "view");
    unsigned int projMatLoc = glGetUniformLocation(defaultShader.getId(), "projection");

    unsigned int originLoc = glGetUniformLocation(defaultShader.getId(), "origin");
    unsigned int sourceMinUVLoc = glGetUniformLocation(defaultShader.getId(), "sourceMinUV");
    unsigned int sourceMaxUVLoc = glGetUniformLocation(defaultShader.getId(), "sourceMaxUV");
    unsigned int sourceSizeLoc = glGetUniformLocation(defaultShader.getId(), "sourceSize");

    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
            }
        }

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture.getId());
        defaultShader.use();
        glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
        glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

        glBindVertexArray(VAO);

        for (int i = 0; i < 10; i++) {
            // Texture source
            glm::vec2 sourceSize = glm::vec2{ 100, 100 };

            float u0 = 0 / (float)texture.getWidth();
            float v0 = 1.f - sourceSize.y / (float)texture.getHeight();
            float u1 = sourceSize.x / (float)texture.getWidth();
            float v1 = 1.f - 0 / (float)texture.getHeight();
            glm::vec2 uv0 = glm::vec2{ u0, v0 };
            glm::vec2 uv1 = glm::vec2{ u1, v1 };

            // Some transformation values
            glm::vec2 origin = glm::vec2{ 0.f, 0.f };
            float scale = 1.f;

            glm::mat4 model(1.f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, glm::vec3{ scale, scale, 1.f });

            // Apply to shader
            glUniform2fv(sourceMinUVLoc, 1, glm::value_ptr(uv0));
            glUniform2fv(sourceMaxUVLoc, 1, glm::value_ptr(uv1));
            glUniform2fv(sourceSizeLoc, 1, glm::value_ptr(sourceSize));
            glUniform2fv(originLoc, 1, glm::value_ptr(origin));
            glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // Present
        SDL_GL_SwapWindow(Engine::getWindow());
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    defaultShader.clean();
    texture.clean();

    Engine::close();
    return 0;
}