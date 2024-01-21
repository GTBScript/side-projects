#include "Includes.h"
#include "UIElements/UIButtons/UIButton.h"
#include "UIElements/UIButtons/StaticUIButton.h"
#include "UIElements/UIButtons/DynamicUIButton.h"
#include "Scenes/Scene.h"


int main() {
    RenderWindow window (VideoMode(Internal::__WIN_WIDTH__, Internal::__WIN_HEIGHT__), "Asteroid Belt", Style::Titlebar | Style::Close);
    window.setFramerateLimit(Internal::__FRAME_RATE__);

    std::stack<Scene> scene_focus;

    Scene * main_menu = MakeScene::make_main_menu();


    while (window.isOpen()) {
        Event event {};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseMoved:
                    break;

                case Event::MouseButtonPressed:
                    break;

                case Event::MouseButtonReleased:
                    break;

                default:
                    break;
            }
        }

        window.clear(Color::Black);
        main_menu->load();
        window.display();
    }


    return 0;
}
