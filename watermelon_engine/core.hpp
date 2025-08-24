#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>

class Engine
{
private:
    static std::string windowTitle;
    static int screenWidth;
    static int screenHeight;

    static SDL_Window* window;
    static SDL_GLContext glContext;
public:
    static void initialize(std::string title, int width, int height);
    static void close();

    static SDL_Window* getWindow() { return window; }
};
