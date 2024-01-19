#include "Includes.h"
#include "UIElements/UIButtons/UIButton.h"
#include "UIElements/UIButtons/StaticUIButton.h"
#include "UIElements/UIButtons/DynamicUIButton.h"


int main() {
    RenderWindow window (VideoMode(Internal::__WIN_WIDTH__, Internal::__WIN_HEIGHT__), "Asteroid Belt", Style::Titlebar | Style::Close);
    window.setFramerateLimit(Internal::__FRAME_RATE__);

    StaticUIButton * button = new StaticUIButton();
    button->assign_window(window);
    button->set_hover_color(Color::Green);
    button->set_press_color(Color::Red);
    button->set_size({100, 100});
    button->set_position({110.f, 110.f});


    while (window.isOpen()) {
        Event event {};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseMoved:
                    button->hover();
                    break;

                case Event::MouseButtonPressed:
                    button->press();
                    break;

                case Event::MouseButtonReleased:
                    button->release();
                    break;

                default:
                    break;
            }
        }

        window.clear(Color::Black);
        button->draw();
        window.display();
    }



    delete button;


    return 0;
}
