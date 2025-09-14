#include <watermelon_engine/core.hpp>
#include <watermelon_engine/gfx.hpp>

glm::vec2 cubePositions[] = {
    glm::vec2(0.0f, 0.0f), 
    glm::vec2(20.f, 50.f), 
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
    Engine::initialize("Some silly test", 1280, 720, false);
    ObjectDrawer::initialize();

    Texture texture("assets/sprites/MainMenuArt.png");
    Texture exitTexture("assets/sprites/Exit.png");

    Camera globalView(1280, 720);
    RenderTarget renderTarget(320, 180);

    Shader sineWaveShader("assets/shaders/default.vert", "assets/shaders/sinewave.frag");

    Camera camera(320, 180);
    camera.setOrigin(glm::vec2{ 320 / 2, 180 / 2 });
    
    glm::vec2 cameraPosition = { 0, 0 };
    const float cameraSpeed = 60;

    Vec2Input cameraDirection({ SDL_SCANCODE_UP }, { SDL_SCANCODE_DOWN }, { SDL_SCANCODE_LEFT }, { SDL_SCANCODE_RIGHT },
        { SDL_GAMEPAD_BUTTON_DPAD_UP }, { SDL_GAMEPAD_BUTTON_DPAD_DOWN }, { SDL_GAMEPAD_BUTTON_DPAD_LEFT }, { SDL_GAMEPAD_BUTTON_DPAD_RIGHT }, THUMBSTICK_SIDE_LEFT);
    
    while (Engine::running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Engine::input(event);
        }
        
        Engine::update();

        globalView.setWidth(Engine::getScreenWidth());
        globalView.setHeight(Engine::getScreenHeight());
        cameraPosition += cameraDirection.getValue() * (float)Engine::getSmoothedDelta() * cameraSpeed;
        camera.setPosition(glm::round(cameraPosition));

        // Rendering
        renderTarget.use();

        ObjectDrawer::clearBackground(Color{ .39f, .58f, .93f });

        for (int i = 0; i < 10; i++) {
            glm::vec2 sourceSize = glm::vec2{ texture.getWidth(), texture.getHeight() };

            ObjectDrawer::drawTexture(camera, texture, cubePositions[i]);
        }

        
        ObjectDrawer::drawTexture(
            camera,
            exitTexture,
            glm::round(cameraPosition),
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
        sineWaveShader.setFloatUniform("amplitude", .2f);
        sineWaveShader.setFloatUniform("frecuency", 10.f);

        renderTarget.unuse();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderTarget.draw(globalView, glm::vec2{ 0, 0 }, glm::vec2{ Engine::getScreenHeight() / renderTarget.getHeight() }, glm::vec2{ 0.f });

        // Present
        SDL_GL_SwapWindow(Engine::getWindow());
    }
    
    ObjectDrawer::clean();
    texture.clean();
    sineWaveShader.clean();
    renderTarget.clean();

    Engine::close();
    return 0;
}