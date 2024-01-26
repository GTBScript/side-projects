#include "Includes.h"
#include "UIElements/UIButtons/UIButton.h"
#include "UIElements/UIButtons/StaticUIButton.h"
#include "UIElements/UIButtons/DynamicUIButton.h"
#include "Scenes/Scene.h"
#include "Scenes/MakeScene.h"
#include "Internal.h"
#include "Actions.h"


int main() {
    RenderWindow window (VideoMode(Internal::__WIN_WIDTH__, Internal::__WIN_HEIGHT__), "Asteroid Belt", Style::Titlebar | Style::Close);
    window.setFramerateLimit(Internal::__FRAME_RATE__);
    Image icon;
    if (!icon.loadFromFile("../../Pictures/icon.png")) {
        throw std::runtime_error("Unable to find app icon");
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    std::unique_ptr<Music> background_music = std::make_unique<Music>();
    Actions::set_music(background_music, "../../Audio/Music/MainTheme.mp3");
    background_music->play();

    Internal::__SCENE_FOCUS__.emplace(MakeScene::MakeMenu(window));

    while (window.isOpen()) {
        Event event {};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseMoved:
                    Internal::__SCENE_FOCUS__.top()->hover();
                    break;

                case Event::MouseButtonPressed:
                    Internal::__SCENE_FOCUS__.top()->press();
                    break;

                case Event::MouseButtonReleased:
                    Internal::__SCENE_FOCUS__.top()->release();
                    break;

                default:
                    break;
            }
        }

        window.clear(Color::Black);
        Internal::__SCENE_FOCUS__.top()->load();
        window.display();
    }


    return 0;
}
