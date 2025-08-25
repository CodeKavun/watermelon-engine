#include "core.hpp"

std::string Engine::windowTitle;
int Engine::screenWidth = 800;
int Engine::screenHeight = 600;

SDL_Window* Engine::window = nullptr;
SDL_GLContext Engine::glContext;

void Engine::initialize(std::string title, int width, int height)
{
    windowTitle = title;
    screenWidth = width;
    screenHeight = height;

    // Init SDL
    SDL_Log("Initializing the engine...");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize the engine! Error message: %s", SDL_GetError());
        SDL_Log("Shutting down the engine...");
        SDL_Quit();
        return;
    }
    SDL_Log("Engine has been initialized!");
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Creating window
    SDL_Log("Creating the window...");
    window = SDL_CreateWindow(windowTitle.c_str(), screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Log("Failed to create the window! Error message: %s", SDL_GetError());
        SDL_Log("Shutting down the engine...");
        SDL_Quit();
        return;
    }
    SDL_Log("Window has been created!");
    
    // Create GL context
    SDL_Log("Creating GL context...");
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("Failed to create GL context! Error message: %s", SDL_GetError());
        SDL_Log("Shutting down the engine...");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    SDL_Log("GL context has been created!");
    
    // Make context as current
    if (!SDL_GL_MakeCurrent(window, glContext)) {
        SDL_Log("Could not make GL context as current! Error message: %s", SDL_GetError());
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    SDL_Log("GL context is now made as current.");
    
    // Enable VSync
    if (!SDL_GL_SetSwapInterval(1)) {
        SDL_Log("Could not enable VSync! Error message: %s", SDL_GetError());
    }
    
    // Load OpenGL functions
    SDL_Log("Loading OpenGL functions...");
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to load OpenGL functions!");
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Show some tech info
    SDL_Log("GL Vendor: %s", (const char*)glGetString(GL_VENDOR));
    SDL_Log("GL Renderer: %s", (const char*)glGetString(GL_RENDERER));
    SDL_Log("GL Version: %s", (const char*)glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, screenWidth, screenHeight);
}

void Engine::close()
{
    SDL_Log("Shutting down the engine...");
    SDL_GL_MakeCurrent(window, nullptr);
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}
