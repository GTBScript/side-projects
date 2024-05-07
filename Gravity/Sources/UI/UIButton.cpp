#include "../../Headers/UI/UIButton.h"

void UIButton::execute(void) {
    if (!this->action) {
        throw std::runtime_error("UIButton::execute() -> Nothing to execute.");
    }

    this->action();
}


void UIButton::attach_window(RenderWindow * __window) {
    this->window = __window;
}


void UIButton::set_sound(const std::string & sound_path) {
    if (!this->sound_buffer) {
        this->sound_buffer = std::make_unique<SoundBuffer>();
    }

    if (!this->sound_buffer->loadFromFile(sound_path)) {
        throw std::runtime_error("UIButton::set_sound() -> Invalid or the sound doesn't exist.");
    }

    if (!this->sound) {
        this->sound = std::make_unique<Sound>();
    }

    this->sound->setBuffer(*this->sound_buffer);
}


void UIButton::set_hover_color(const Color color) {
    if (!this->shape) {
        throw std::runtime_error("UIButton::set_base_color() -> Nothing to set base color to.");
    }

    if (!this->hover_color) {
        this->hover_color = std::make_unique<Color>();
    }

    *this->hover_color = color;
}


void UIButton::set_base_color(const Color color) {
    if (!this->shape) {
        throw std::runtime_error("UIButton::set_base_color() -> Nothing to set base color to.");
    }

    if (!this->base_color) {
        this->base_color = std::make_unique<Color>();
    }

    *this->base_color = color;
    this->shape->setFillColor(color);
}


void UIButton::set_press_color(const Color color) {
    if (!this->shape) {
        throw std::runtime_error("UIButton::set_base_color() -> Nothing to set base color to.");
    }

    if (!this->press_color) {
        this->press_color = std::make_unique<Color>();
    }

    *this->press_color = color;
}


void UIButton::set_text(const std::string & __text) {
    if (!this->text) {
        this->text = std::make_unique<Text>();
    }

    this->text->setString(__text);
    this->center_text();
}


void UIButton::set_size(Vector2f size) {
    this->shape->setSize(size);
    this->center_text();
}


void UIButton::set_texture(const std::string & texture_path) {
    if (!this->texture) {
        this->texture = std::make_unique<Texture>();
    }

    if (!this->texture->loadFromFile(texture_path)) {
        throw std::runtime_error("UIButton::set_texture() -> Invalid or the texture doesn't exist.");
    }

    this->shape->setTexture(&(*this->texture));
}


void UIButton::set_position(Vector2f position) {
    if (!this->shape) {
        throw std::runtime_error("UIButton::set_position() -> Nothing to set position.");
    }

    this->shape->setPosition(position);
    this->center_text();
}


void UIButton::set_font(const std::string & font_path) {
    if (!this->text) {
        throw std::runtime_error("UIButton::set_font() -> Nowhere to set font.");
    }

    if (!this->font) {
        this->font = std::make_unique<Font>();
    }

    if (!this->font->loadFromFile(font_path)) {
        throw std::runtime_error("UIButton::set_font() -> Invalid or the font doesn't exist.");
    }

    this->text->setFont(*this->font);
}


void UIButton::set_font_size(const unsigned int size) {
    if (!this->text) {
        throw std::runtime_error("UIButton::set_font_size() -> Unable to set size to nonexistent item.");
    }

    this->text->setCharacterSize(size);
    this->center_text();
}


UIButton::UIButton(RenderWindow * window) {
    this->window = window;
    this->shape = std::make_unique<RectangleShape>();
}

void UIButton::set_text_color(Color color) noexcept {
    if (this->text) {
        this->text->setFillColor(color);
    }
}


Text * UIButton::self_text(void) noexcept {
    return this->text.get();
}


RectangleShape * UIButton::self_shape(void) noexcept {
    return this->shape.get();
}


