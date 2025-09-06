#include <watermelon_engine/core.hpp>
#include <watermelon_engine/gfx.hpp>

glm::vec2 cubePositions[] = {
    glm::vec2(0.0f, 0.0f), 
    glm::vec2( 20.f, 50.f), 
    glm::vec2(150.f, 220.f),  
    glm::vec2(380.f, 200.f),  
    glm::vec2(240.f, 40.f),  
    glm::vec2(1100.f, 30.f),  
    glm::vec2(1150.f, 50.f),  
    glm::vec2(1000.f, 200.f), 
    glm::vec2(150.f, 200.f), 
    glm::vec2(130.f, 160.f)  
};

int main(int argc, char** argv) {
    Engine::initialize("Some silly test", 1280, 720);
    ObjectDrawer::initialize();

    Texture texture("assets/sprites/MainMenuArt.png");
    Texture exitTexture("assets/sprites/Exit.png");

    Shader sineWaveShader("assets/shaders/default.vert", "assets/shaders/sinewave.frag");

    Camera camera(1280, 720);
    camera.setZoom(3);
    
    glm::vec2 cameraDirection = { 0, 0 };
    glm::vec2 cameraPosition = { 0, 0 };
    const float cameraSpeed = 60;
    
    while (Engine::running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Engine::input(event);
        }
        
        Engine::update();

        const bool* state = SDL_GetKeyboardState(nullptr);

        if (state[SDL_SCANCODE_UP]) cameraDirection.y = -1;
        if (state[SDL_SCANCODE_DOWN]) cameraDirection.y = 1;
        if (state[SDL_SCANCODE_LEFT]) cameraDirection.x = -1;
        if (state[SDL_SCANCODE_RIGHT]) cameraDirection.x = 1;
                
        camera.setWidth(Engine::getScreenWidth());
        camera.setHeight(Engine::getScreenHeight());
        camera.setOrigin(glm::vec2{ Engine::getScreenWidth() / 2, Engine::getScreenHeight() / 2 });
        cameraPosition += cameraDirection * Engine::getDeltaTime() * cameraSpeed;
        camera.setPosition(cameraPosition);

        cameraDirection = glm::vec2{ 0, 0 };

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 10; i++) {
            glm::vec2 sourceSize = glm::vec2{ texture.getWidth(), texture.getHeight() };

            ObjectDrawer::drawTexture(camera, texture, cubePositions[i]);
        }

        
        ObjectDrawer::drawTexture(
            camera,
            exitTexture,
            cameraPosition,
            glm::vec2{ exitTexture.getWidth() / 2, exitTexture.getHeight() / 2 },
            glm::vec2{ 1 },
            0,
            nullptr,
            false, false,
            sineWaveShader,
            0.f
        );

        sineWaveShader.setFloatUniform("time", Engine::getTime());
        sineWaveShader.setFloatUniform("speed", 2.f);
        sineWaveShader.setFloatUniform("amplitude", .015f);
        sineWaveShader.setFloatUniform("frecuency", 30.f);

        // Present
        SDL_GL_SwapWindow(Engine::getWindow());
    }
    
    ObjectDrawer::clean();
    texture.clean();
    sineWaveShader.clean();

    Engine::close();
    return 0;
}