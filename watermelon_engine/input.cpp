#include "input.hpp"
#include "utils.hpp"

const bool* InputHandler::currentKeyboardState = nullptr;
const bool* InputHandler::previousKeyboardState = nullptr;
SDL_Gamepad* InputHandler::gamepad = nullptr;

void InputHandler::checkGamepadConnections(const SDL_Event &event)
{
    switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED: {
            if (!gamepad) {
                gamepad = SDL_OpenGamepad(event.gdevice.which);
                if (gamepad) {
                    SDL_Log("Connected gamepad device: %s", SDL_GetGamepadName(gamepad));
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            if (gamepad && event.gdevice.which == SDL_GetGamepadID(gamepad)) {
                SDL_CloseGamepad(gamepad);
                gamepad = nullptr;
                SDL_Log("Gamepad is disconnected");
            }
            break;
        }
    }
}

void InputHandler::update()
{
    previousKeyboardState = currentKeyboardState;
    currentKeyboardState = SDL_GetKeyboardState(nullptr);
}

bool ButtonInput::isPressed()
{
    bool pressed = false;

    if (InputHandler::getGamepad()) {
        for (const SDL_GamepadButton& button : buttons) {
            pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) break;
        }
    }
    else {
        for (const SDL_Scancode& key : keys) {
            pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) break;
        }
    }

    return pressed;
}

bool ButtonInput::isReleased()
{
    bool released = false;

    if (InputHandler::getGamepad()) {
        for (const SDL_GamepadButton& button : buttons) {
            released = !SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (released) break;
        }
    }
    else {
        for (const SDL_Scancode& key : keys) {
            released = !InputHandler::getCurrentKeyboardState()[key];
            if (released) break;
        }
    }

    return released;
}

bool ButtonInput::isJustPressed(const SDL_Event& event)
{
    bool justPressed = false;

    if (InputHandler::getGamepad()) {
        if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
            for (const SDL_GamepadButton& button : buttons) {
                if (event.gbutton.button == button) justPressed = true;
                if (justPressed) break;
            }
        }
    }
    else {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            for (const SDL_Scancode& key : keys) {
                if (!event.key.repeat && event.key.scancode == key) justPressed = true;
                if (justPressed) break;
            }
        }
    }

    return justPressed;
}

bool ButtonInput::isJustReleased(const SDL_Event& event)
{
    bool justReleased = false;

    if (InputHandler::getGamepad()) {
        if (event.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
            for (const SDL_GamepadButton& button : buttons) {
                if (event.gbutton.button == button) justReleased = true;
                if (justReleased) break;
            }
        }
    }
    else {
        if (event.type == SDL_EVENT_KEY_UP) {
            for (const SDL_Scancode& key : keys) {
                if (!event.key.repeat && event.key.scancode == key) justReleased = true;
                if (justReleased) break;
            }
        }
    }

    return justReleased;
}

float FloatInput::getValue()
{
    float direction = 0.f;
    
    if (InputHandler::getGamepad()) {
        // If using thumbsticks
        switch (thumbstickSide) {
            case THUMBSTICK_SIDE_LEFT: {
                switch (thumbstickAxis) {
                    case THUMBSTICK_AXIS_X: {
                        direction = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_LEFTX), 32767.0f);
                        break;
                    }
                    case THUMBSTICK_AXIS_Y: {
                        direction = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_LEFTY), 32767.0f);
                        break;
                    }
                }
                break;
            }
            case THUMBSTICK_SIDE_RIGHT: {
                switch (thumbstickAxis) {
                    case THUMBSTICK_AXIS_X: {
                        direction = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_RIGHTX), 32767.0f);
                        break;
                    }
                    case THUMBSTICK_AXIS_Y: {
                        direction = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_RIGHTY), 32767.0f);
                        break;
                    }
                }
                break;
            }
        }
        
        for (const SDL_GamepadButton& button : negativeButtons) {
            bool pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) {
                direction -= 1.f;
                break;
            }
        }
    
        for (const SDL_GamepadButton& button : positiveButtons) {
            bool pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) {
                direction += 1.f;
                break;
            }
        }

    }
    else {
        for (const SDL_Scancode& key : negativeKeys) {
            bool pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) {
                direction -= 1.f;
                break;
            }
        }
    
        for (const SDL_Scancode& key : positiveKeys) {
            bool pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) {
                direction += 1.f;
                break;
            }
        }
    }

    return direction;
}

glm::vec2 Vec2Input::getValue()
{
    glm::vec2 direction = { 0, 0 };

    if (InputHandler::getGamepad()) {
        // Again, for using thumbsticks
        float thumbstickAxisX = 0;
        float thumbstickAxisY = 0;
        switch (thumbstickSide) {
            case THUMBSTICK_SIDE_LEFT: {
                thumbstickAxisX = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_LEFTX), 32767.0f);
                thumbstickAxisY = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_LEFTY), 32767.0f);
                break;
            }
            case THUMBSTICK_SIDE_RIGHT: {
                thumbstickAxisX = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_RIGHTX), 32767.0f);
                thumbstickAxisY = normalizeAxis(SDL_GetGamepadAxis(InputHandler::getGamepad(), SDL_GAMEPAD_AXIS_RIGHTY), 32767.0f);
                break;
            }
        }

        for (const SDL_GamepadButton& button : upButtons) {
            bool pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) {
                direction.y -= 1.f;
                break;
            }
        }
    
        for (const SDL_GamepadButton& button : downButtons) {
            bool pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) {
                direction.y += 1.f;
                break;
            }
        }
    
        for (const SDL_GamepadButton& button : leftButtons) {
            bool pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) {
                direction.x -= 1.f;
                break;
            }
        }
    
        for (const SDL_GamepadButton& button : rightButtons) {
            bool pressed = SDL_GetGamepadButton(InputHandler::getGamepad(), button);
            if (pressed) {
                direction.x += 1.f;
                break;
            }
        }

        if (std::abs(thumbstickAxisX) >= .2f || std::abs(thumbstickAxisY) >= .2f) {
            direction.x = thumbstickAxisX;
            direction.y = thumbstickAxisY;
        }
        else {
            if (direction != glm::vec2{ 0, 0 }) direction = glm::normalize(direction);
        }
    }
    else {
        for (const SDL_Scancode& key : upKeys) {
            bool pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) {
                direction.y -= 1.f;
                break;
            }
        }
    
        for (const SDL_Scancode& key : downKeys) {
            bool pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) {
                direction.y += 1.f;
                break;
            }
        }
    
        for (const SDL_Scancode& key : leftKeys) {
            bool pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) {
                direction.x -= 1.f;
                break;
            }
        }
    
        for (const SDL_Scancode& key : rightKeys) {
            bool pressed = InputHandler::getCurrentKeyboardState()[key];
            if (pressed) {
                direction.x += 1.f;
                break;
            }
        }

        if (direction != glm::vec2{ 0, 0 }) direction = glm::normalize(direction);
    }

    return direction;
}
