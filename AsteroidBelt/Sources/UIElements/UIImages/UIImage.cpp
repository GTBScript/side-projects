#include "UIImage.h"

UIImage::UIImage() {
    this->shape = std::make_unique<RectangleShape>();
}

void UIImage::set_position(const Vector2f pos) {
    this->shape->setPosition(pos);
}

void UIImage::set_size(const Vector2f size) {
    this->shape->setSize(size);
}

void UIImage::set_texture(const std::string &path) {
    if (!this->texture) {
        this->texture = std::make_unique<Texture>();
    }

    if (!this->texture->loadFromFile(path)) {
        throw std::runtime_error("Invalid or the image does not exist. ERROR at UIImage.cpp");
    }

    this->shape->setTexture(&(*this->texture));
}


void UIImage::assign_window(RenderWindow & window_) {
    this->window = &window_;
}

void UIImage::draw() {
    this->window->draw(*this->shape);
}

Texture & UIImage::get_texture(void) {
    return std::ref(*this->texture);
}
