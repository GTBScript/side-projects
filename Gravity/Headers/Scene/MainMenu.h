#ifndef GRAVITY_MAINMENU_H
#define GRAVITY_MAINMENU_H
#include "Scene.h"

class MainMenu : public Scene {
protected:


private:
    std::unique_ptr<Texture> background_image = nullptr;
    std::unique_ptr<Sprite> sprite = nullptr;

public:
    MainMenu() : Scene() {}

    void set_background_image(const std::string & path);
    void inline load(void) noexcept override;
};


inline void MainMenu::load(void) noexcept {
    this->window->draw(*this->sprite);
    Scene::load();
}

#endif //GRAVITY_MAINMENU_H
