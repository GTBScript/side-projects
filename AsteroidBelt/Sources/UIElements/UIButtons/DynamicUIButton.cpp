#include "DynamicUIButton.h"

DynamicUIButton::DynamicUIButton() : StaticUIButton() {}


void DynamicUIButton::set_base_texture(const std::string &texture_path) {
    if (!this->base_texture) {
        this->base_texture = std::make_unique<Texture>();
    }

    if (!this->base_texture->loadFromFile(texture_path)) {
        throw std::runtime_error("Invalid or the texture does not exist.");
    }

    this->shape->setTexture(&(*this->base_texture));
}


void DynamicUIButton::set_hover_texture(const std::string &texture_path) {
    if (!this->hover_texture) {
        this->hover_texture = std::make_unique<Texture>();
    }

    if (!this->hover_texture->loadFromFile(texture_path)) {
        throw std::runtime_error("Invalid or the texture does not exist.");
    }
}


void DynamicUIButton::set_press_texture(const std::string &texture_path) {
    if (!this->press_texture) {
        this->press_texture = std::make_unique<Texture>();
    }

    if (!this->press_texture->loadFromFile(texture_path)) {
        throw std::runtime_error("Invalid or the texture does not exist.");
    }
}


void DynamicUIButton::set_position(Vector2f position) {
    this->shape->setPosition(position);
}


void DynamicUIButton::hover() noexcept {
    if (this->is_mouse_over_shape()) {
        if (!this->pressed) {
            if (this->hover_texture) {
                this->shape->setTexture(&(*this->hover_texture));
            } else if (this->hover_color) {
                this->shape->setFillColor(*this->hover_color);
            }

            this->hovered = true;
        }
    } else {
        if (this->base_texture) {
            this->shape->setTexture(&(*this->base_texture));
        } else if (this->base_color) {
            this->shape->setFillColor(*this->base_color);
        }

        this->hovered = false;
        this->pressed = false;
    }
}


void DynamicUIButton::press() noexcept {
    if (this->hovered) {
        if (this->press_texture) {
            this->shape->setTexture(&(*this->press_texture));
        } else if (this->press_color) {
            this->shape->setFillColor(*this->press_color);
        }

        this->execute();
        this->pressed = true;
    }
}


void DynamicUIButton::release() noexcept {
    if (this->hovered) {
        if (this->pressed) {
            if (this->hover_texture) {
                this->shape->setTexture(&(*this->hover_texture));
            } else if (this->hover_color) {
                this->shape->setFillColor(*this->hover_color);
            }
        }
    } else {
        if (this->base_texture) {
            this->shape->setTexture(&(*this->base_texture));
        } else if (this->base_color) {
            this->shape->setFillColor(*this->base_color);
        }
    }

    this->pressed = false;
}