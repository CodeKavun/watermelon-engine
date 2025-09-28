#pragma once
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>
#include <deque>
#include "input.hpp"

#define SMOOTHING .15f

class Engine
{
private:
    static std::string windowTitle;
    static int screenWidth;
    static int screenHeight;

    static SDL_Window* window;
    static SDL_GLContext glContext;

    static Uint64 currentTime;
    static Uint64 lastTime;
    static double deltaTime;
    static double smoothedDeltaTime;
public:
    static bool running;

    static void initialize(std::string title, int width, int height, bool fullscreen);
    static void update();
    static void input(SDL_Event& event);
    static void close();

    static std::string getTitle() { return SDL_GetWindowTitle(window); }
    static void setTitle(std::string title) { SDL_SetWindowTitle(window, title.c_str()); }

    static int getScreenWidth() { return screenWidth; }
    static int getScreenHeight() { return screenHeight; }
    static void setScreenSize(int width, int height) {
        screenWidth = width;
        screenHeight = height;
        SDL_SetWindowSize(window, screenWidth, screenHeight);
    }

    static SDL_Window* getWindow() { return window; }

    static float getTime() { return SDL_GetTicks() / 1000.f; }
    static double getDeltaTime() { return deltaTime; }
    static double getSmoothedDelta() { return smoothedDeltaTime; }
};
