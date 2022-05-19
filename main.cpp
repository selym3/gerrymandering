#include <SFML/Graphics.hpp>
#include <iostream>
#include <ostream>

template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& rhs)
{
    return os << "[ " << rhs.x << ", " << rhs.y << " ]";
}

#include "camera.hpp"
#include "mouse.hpp"

namespace gm 
{

struct engine;

struct control_mode
{
    virtual void handle_event(const engine& engine, const sf::Event& event) = 0;
    virtual void execute(const engine& engine) = 0;
};

struct pan_zoom : public control_mode
{

    void handle_event(const engine& engine, const sf::Event& event) 
    {

    }

    void execute(const engine& engine)
    {
        
    }

};

}

namespace gm
{

struct engine
{
public:
    sf::RenderWindow window;

    engine() :
        window { sf::VideoMode(640, 480), "gerrymandering", sf::Style::None }
    {
    }

public:
    void handle_event(const sf::Event& event) 
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
            return;
        }


    }

public:
    bool is_open() const { return window.isOpen(); }
    
    void update() 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            handle_event(event);
        }


    }
};

}

int main2() 
{
    gm::engine engine;

    while (engine.is_open())
    {
        engine.update(); 
    }

    return 0;
}

int main(int argc, char** argv)
{
    sf::RenderWindow window 
    {
        sf::VideoMode(640, 480),
        "gerrymandering",
        sf::Style::None
    };

    gm::camera camera
    {
        gm::window(gm::vec2d::from(window.getSize())),
        gm::window(gm::vec2d{ 20, 20 })
    };
    // camera -= camera.screen_to_world(gm::vec2d::from(window.getSize())/2.0);

    gm::vec2d anchor{0, 0};
    Mouse mouse;

    // float z = 1.0; 
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::MouseWheelScrolled)     
            {
                double scalar = event.mouseWheelScroll.delta > 0 ? 1.1 : 0.9;
                auto pos = gm::vec2d::from(sf::Mouse::getPosition(window));
                auto before = camera.screen_to_world(pos);

                camera *= scalar;
                camera -= camera.screen_to_world(pos) - before;
            }

        }
        
        using keys = sf::Keyboard;
        camera += gm::vec2d(
            (keys::isKeyPressed(keys::A) - keys::isKeyPressed(keys::D)),
            (keys::isKeyPressed(keys::W) - keys::isKeyPressed(keys::S))
        ) * (1 / 60.0);

        using ms = sf::Mouse;
        auto pos = gm::vec2d::from(ms::getPosition(window));
        if (mouse.isPressed(ms::Button::Left)) {
            anchor = pos;
        }
        else if (mouse.isHeld(ms::Button::Left)) {
            auto d = camera.screen_to_world(pos) - camera.screen_to_world(anchor);
            camera -= d;
            anchor = pos;
        }

        mouse.update();

        // sf::View view = window.getDefaultView();
        // view.zoom(z*=0.9);
        // window.setView(view);

        window.clear(sf::Color::White);

        sf::RectangleShape rect;
        // rect.setPosition(view.getCenter());
        // rect.setSize({10, 10});

        // std::cout << view.getCenter() << "\n";
        
        // std::cout << window.mapCoordsToPixel(rect.getPosition()) << "\n";
        gm::vec2d rpos{0, 0};
        gm::vec2d rsize{10, 10};
        rect.setPosition(camera.world_to_screen(rpos).to<float>());
        rect.setSize((camera.world_to_screen(rpos + rsize) - camera.world_to_screen(rpos)).to<float>());
        rect.setFillColor(sf::Color::Blue);
        
        window.draw(rect);

        window.display();
        sf::sleep(sf::seconds(1/60.0));
    }

    return 0;
}