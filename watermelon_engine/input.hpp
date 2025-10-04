#include <SDL3/SDL.h>
#include <vector>
#include <glm/glm.hpp>

enum ThumbstickSide {
    THUMBSTICK_SIDE_LEFT,
    THUMBSTICK_SIDE_RIGHT
};

enum ThumbstickAxis {
    THUMBSTICK_AXIS_X,
    THUMBSTICK_AXIS_Y,
};

class InputHandler
{
private:
    static const bool* currentKeyboardState;
    static const bool* previousKeyboardState;
    static SDL_Gamepad* gamepad;
public:
    static const bool* getCurrentKeyboardState() {
        return currentKeyboardState;
    }

    static const bool* getPreviousKeyboardState() {
        return previousKeyboardState;
    }

    static SDL_Gamepad* getGamepad() {
        return gamepad;
    }

    static void checkGamepadConnections(const SDL_Event& event);
    static void update();
};

class ButtonInput
{
private:
    std::vector<SDL_Scancode> keys;
    std::vector<SDL_GamepadButton> buttons;
public:
    ButtonInput() = default;
    ButtonInput(std::vector<SDL_Scancode> keys, std::vector<SDL_GamepadButton> buttons) : keys(keys), buttons(buttons) {}

    bool isPressed();
    bool isReleased();
    bool isJustPressed(const SDL_Event& event);
    bool isJustReleased(const SDL_Event& event);
};

class FloatInput
{
private:
    std::vector<SDL_Scancode> negativeKeys;
    std::vector<SDL_Scancode> positiveKeys;

    std::vector<SDL_GamepadButton> negativeButtons;
    std::vector<SDL_GamepadButton> positiveButtons;

    ThumbstickSide thumbstickSide;
    ThumbstickAxis thumbstickAxis;
public:
    FloatInput() = default;
    FloatInput(std::vector<SDL_Scancode> negativeKeys, std::vector<SDL_Scancode> positiveKeys, std::vector<SDL_GamepadButton> negativeButtons, std::vector<SDL_GamepadButton> positiveButtons, ThumbstickSide thumbstickSide, ThumbstickAxis thumbstickAxis)
        : negativeKeys(negativeKeys), positiveKeys(positiveKeys), negativeButtons(negativeButtons), positiveButtons(positiveButtons), thumbstickSide(thumbstickSide), thumbstickAxis(thumbstickAxis) {}
    
    float getValue();
};

class Vec2Input
{
private:
    std::vector<SDL_Scancode> upKeys;
    std::vector<SDL_Scancode> downKeys;
    std::vector<SDL_Scancode> leftKeys;
    std::vector<SDL_Scancode> rightKeys;

    std::vector<SDL_GamepadButton> upButtons;
    std::vector<SDL_GamepadButton> downButtons;
    std::vector<SDL_GamepadButton> leftButtons;
    std::vector<SDL_GamepadButton> rightButtons;

    ThumbstickSide thumbstickSide;
public:
    Vec2Input() = default;
    Vec2Input(std::vector<SDL_Scancode> upKeys, std::vector<SDL_Scancode> downKeys, std::vector<SDL_Scancode> leftKeys, std::vector<SDL_Scancode> rightKeys,
        std::vector<SDL_GamepadButton> upButtons, std::vector<SDL_GamepadButton> downButtons, std::vector<SDL_GamepadButton> leftButtons, std::vector<SDL_GamepadButton> rightButtons, ThumbstickSide thumbstickSide)
        : upKeys(upKeys), downKeys(downKeys), leftKeys(leftKeys), rightKeys(rightKeys), upButtons(upButtons), downButtons(downButtons), leftButtons(leftButtons), rightButtons(rightButtons), thumbstickSide(thumbstickSide) {}
    
    glm::vec2 getValue();
};