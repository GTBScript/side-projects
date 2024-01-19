#include "UIButton.h"

UIButton::UIButton() {
    this->shape = std::make_unique<RectangleShape>();
}


void UIButton::assign_sound(const std::string & sound_path) {
    if (!this->sound_buffer) {
        this->sound_buffer = std::make_unique<SoundBuffer>();
    }

    if (!this->sound_buffer->loadFromFile(sound_path)) {
        throw std::runtime_error("Sound path is incorrect or the file does not exist.");
    }

    if (!this->sound) {
        this->sound = std::make_unique<Sound>();
    }

    this->sound->setBuffer(*this->sound_buffer);
}


void UIButton::set_position(Vector2f position) {
    this->shape->setPosition(position);
}


void UIButton::set_hover_color(Color color) {
    if (!this->hover_color) {
        this->hover_color = std::make_unique<Color>();
    }

    *this->hover_color = color;
}


void UIButton::draw() {
    this->window->draw(*this->shape);
}


bool UIButton::is_mouse_over_shape() {
    const Vector2i mouse_pos = Mouse::getPosition(*this->window);
    return this->shape->getGlobalBounds().contains(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));
}


void UIButton::assign_window(RenderWindow & _window) {
    this->window = &_window;
}


template <typename Func> void UIButton::set_action(Func f) {
    this->action = [this, f](){f();};
}


void UIButton::execute() {
    if (this->action) {
        this->action();
    }
}


void UIButton::set_size(Vector2f size) {
    this->shape->setSize(size);
}


UIButton::~UIButton() {
    this->window = nullptr;
}

void UIButton::press() noexcept {}
void UIButton::hover() noexcept {}
void UIButton::release() noexcept {}