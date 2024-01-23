#include "StaticUIButton.h"

StaticUIButton::StaticUIButton() : UIButton() {
    this->base_color = std::make_unique<Color>(Color::White);
}


void StaticUIButton::set_base_color(Color color) {
    *this->base_color = color;
    this->shape->setFillColor(color);
}


void StaticUIButton::set_press_color(Color color) {
    *this->press_color = color;
}


void StaticUIButton::set_string(const std::string & string) noexcept {
    if (!this->text) {
        this->text = std::make_unique<Text>();
    }
    this->text->setString(string);
}


void StaticUIButton::set_font(const std::string & font_path) {
    if (!this->font) {
        this->font = std::make_unique<Font>();
    }

    if (!this->font->loadFromFile(font_path)) {
        throw std::runtime_error("Invalid or the font does not exist.");
    }
    this->text->setFont(*this->font);
}


void StaticUIButton::set_font_size(const unsigned size) noexcept {
    this->text->setCharacterSize(size);
}


void StaticUIButton::set_position(Vector2f position) {
    this->shape->setPosition(position);

    if (this->text) {
        FloatRect textBounds = this->text->getLocalBounds();
        this->text->setOrigin(textBounds.left + textBounds.width/2.0f, textBounds.top + textBounds.height/2.0f);
        this->text->setPosition(position.x + this->shape->getGlobalBounds().width/2.0f, position.y + this->shape->getGlobalBounds().height/2.0f);
    }
}


void StaticUIButton::hover() noexcept {
    if (this->is_mouse_over_shape()) {
        if (!this->pressed) {
            if (this->hover_color) {
                this->shape->setFillColor(*this->hover_color);
            }

            this->hovered = true;
        }
    } else {
        this->shape->setFillColor(*this->base_color);
        this->hovered = false;
        this->pressed = false;
    }
}


void StaticUIButton::press() noexcept {
    if (this->hovered) {
        this->shape->setFillColor(*this->press_color);
        this->pressed = true;
    }
}


void StaticUIButton::release() noexcept {
    if (this->hovered) {
        if (this->pressed) {
            this->shape->setFillColor(*this->hover_color);
        }
    } else {
        this->shape->setFillColor(*this->base_color);
    }

    this->pressed = false;
}



