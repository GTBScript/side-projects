#include "../../Headers/Scene/MainMenu.h"

void MainMenu::set_background_image(const std::string & path) {
    if (!this->background_image) {
        this->background_image = std::make_unique<Texture>();
    }

    if (!this->background_image->loadFromFile(path)) {
        throw std::runtime_error("MainMenu::set_background_image() -> Invalid or the image doesn't exist.");
    }

    if (!this->sprite) {
        this->sprite = std::make_unique<Sprite>();
    }

    this->sprite->setTexture(*this->background_image);
    const Vector2u texture_size = this->background_image->getSize();
    const Vector2u window_size = this->window->getSize();
    const float scale_x = static_cast<float>(window_size.x) / static_cast<float>(texture_size.x);
    const float scale_y = static_cast<float>(window_size.y) / static_cast<float>(texture_size.y);
    this->sprite->setScale(scale_x, scale_y);
}



