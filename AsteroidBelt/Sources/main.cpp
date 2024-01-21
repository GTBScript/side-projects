#include "Includes.h"
#include "UIElements/UIButtons/UIButton.h"
#include "UIElements/UIButtons/StaticUIButton.h"
#include "UIElements/UIButtons/DynamicUIButton.h"
#include "Scenes/Scene.h"
#include "Scenes/MakeScene.h"


int main() {
    RenderWindow window (VideoMode(Internal::__WIN_WIDTH__, Internal::__WIN_HEIGHT__), "Asteroid Belt", Style::Titlebar | Style::Close);
    window.setFramerateLimit(Internal::__FRAME_RATE__);

    std::stack<std::unique_ptr<Scene>> scene_focus;

    scene_focus.emplace(MakeScene::MakeMenu(window));


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
        scene_focus.top()->load();
        window.display();
    }


    return 0;
}
