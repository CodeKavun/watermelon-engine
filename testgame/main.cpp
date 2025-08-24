#include <watermelon_engine/core.hpp>

int main(int argc, char** argv) {
    Engine::initialize("Some silly test", 1280, 720);

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

        // Present
        SDL_GL_SwapWindow(Engine::getWindow());
    }

    Engine::close();
    return 0;
}