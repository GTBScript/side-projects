#include "Includes.h"
#include "UIElements/UIButtons/UIButton.h"
#include "UIElements/UIButtons/StaticUIButton.h"
#include "UIElements/UIButtons/DynamicUIButton.h"
#include "Scenes/Scene.h"
#include "Scenes/MakeScene.h"


int main() {
    RenderWindow window (VideoMode(Internal::__WIN_WIDTH__, Internal::__WIN_HEIGHT__), "Asteroid Belt", Style::Titlebar | Style::Close);
    window.setFramerateLimit(Internal::__FRAME_RATE__);
    Image icon;
    if (!icon.loadFromFile("../../Pictures/icon.png")) {
        throw std::runtime_error("Unable to find app icon");
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

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
                    scene_focus.top()->hover();
                    break;

                case Event::MouseButtonPressed:
                    scene_focus.top()->press();
                    break;

                case Event::MouseButtonReleased:
                    scene_focus.top()->release();
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
