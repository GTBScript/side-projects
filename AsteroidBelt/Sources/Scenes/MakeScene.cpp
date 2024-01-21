#include "MakeScene.h"

std::unique_ptr<Scene> MakeScene::MakeMenu(RenderWindow & window) {
    std::unique_ptr<Scene> menu = std::make_unique<Scene>();
    menu->attach_window(window);
    menu->set_animated_background("../../Pictures/MainMenuBackground/bckg-");
    menu->set_background_music("../../Audio/Music/MainTheme.mp3");

    return menu;
}
