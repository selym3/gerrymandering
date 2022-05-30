#include "./behavior.hpp"
#include <SFML/Window/Mouse.hpp>

#include "../util/vec2.hpp"
#include "./engine.hpp"
// #include "./camera.hpp"

using namespace gl;

behavior::~behavior() { }

pan_zoom::pan_zoom() : anchor { 0.0, 0.0 }
{
}

void pan_zoom::handle_event(engine& en, const sf::Event& event)
{
    if (event.type == sf::Event::MouseWheelScrolled)     
    {
        double scalar = event.mouseWheelScroll.delta > 0 ? 1.1 : 0.9;
        auto pos = gm::vec2d::from(sf::Mouse::getPosition(en.get_window()));
        auto before = en.get_camera().screen_to_world(pos);

        en.get_camera() *= gm::vec2d(scalar);
        en.get_camera() -= en.get_camera().screen_to_world(pos) - before;
    }
}

using ms = sf::Mouse;

void pan_zoom::execute(engine& en)
{
    auto pos = gm::vec2d::from(ms::getPosition(en.get_window()));
    if (en.get_mouse().isPressed(ms::Button::Left)) {
        anchor = pos;
    } else if (en.get_mouse().isHeld(ms::Button::Left)) {
        auto d = en.get_camera().screen_to_world(pos) - en.get_camera().screen_to_world(anchor);
        en.get_camera() -= d;
        anchor = pos;
    }
    en.get_mouse().update();
}
