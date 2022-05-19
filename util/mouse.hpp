#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#include <SFML/Window/Mouse.hpp>
#include <array>

class Mouse
{
    using Button = sf::Mouse::Button;
    using ButtonCache = std::array<bool, Button::ButtonCount>;

    ButtonCache _buttons, _old;

public:

    Mouse()
    { update(); }

    // POSITION //

    sf::Vector2i getPosition(const sf::Window& window) const 
    { return sf::Mouse::getPosition(window); }

    // BUTTONS // 

    template <typename T>
    bool isPressed(const T& button) 
    { return _buttons[static_cast<std::size_t>(button)] && !_old[static_cast<std::size_t>(button)]; }

    template <typename T>
    bool isHeld(const T& button) 
    { return _buttons[static_cast<std::size_t>(button)] && _old[static_cast<std::size_t>(button)]; }

    template <typename T>
    bool isReleased(const T& button) 
    { return !_buttons[static_cast<std::size_t>(button)] && _old[static_cast<std::size_t>(button)]; }

    template <typename T>
    bool isDown(const T& button)
    { return _buttons[static_cast<std::size_t>(button)]; }

    void update()
    {
        _old = _buttons;
        for (std::size_t i = 0; i < Button::ButtonCount; ++i)
        {
            _buttons[i] = sf::Mouse::isButtonPressed(static_cast<Button>(i));
        }
    }

};

#endif