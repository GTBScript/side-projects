#ifndef GRAVITY_UIBUTTON_H
#define GRAVITY_UIBUTTON_H
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <functional>

using namespace sf;

class UIButton {
private:
    std::unique_ptr<RectangleShape> shape = nullptr;
    std::unique_ptr<Sound> sound = nullptr;
    std::unique_ptr<SoundBuffer> sound_buffer = nullptr;
    std::unique_ptr<Color> hover_color = nullptr;
    std::unique_ptr<Color> press_color = nullptr;
    std::unique_ptr<Color> base_color = nullptr;
    std::unique_ptr<Texture> texture = nullptr;
    std::unique_ptr<Text> text = nullptr;
    std::unique_ptr<Font> font = nullptr;

    RenderWindow * window = nullptr;

    bool is_pressed = false;
    bool is_hovered = false;

    std::function<void()> action;

    inline bool is_mover_over_shape(void) const {
        const Vector2i mouse_pos = Mouse::getPosition(*this->window);
        return this->shape->getGlobalBounds().contains(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));
    }

    inline void center_text(void);

public:
    explicit UIButton(RenderWindow * window = nullptr);

    inline void draw(void) const noexcept;
    inline void press(void);
    inline void hover(void) noexcept;
    inline void release(void);

    RectangleShape * self_shape(void) noexcept;
    Text * self_text(void) noexcept;
    void execute(void);
    void attach_window(RenderWindow * __window);
    void set_sound(const std::string & sound_path);
    void set_hover_color(Color color);
    void set_base_color(Color color);
    void set_press_color(Color color);
    void set_text(const std::string & __text);
    void set_text_color(Color color) noexcept;
    void set_size(Vector2f size);
    void set_texture(const std::string & texture_path);
    void set_position(Vector2f position);
    void set_font(const std::string & font_path);
    void set_font_size(unsigned size);

    template <typename Func> void attach_action(Func f) {
        this->action = f;
    }
};



inline void UIButton::center_text(void) {
    if (!this->text || !this->shape) {
        return;
    }

    FloatRect text_rect = this->text->getLocalBounds();
    this->text->setOrigin(text_rect.left + text_rect.width/2.0f, text_rect.top + text_rect.height/2.0f);
    FloatRect shape_bounds = this->shape->getGlobalBounds();
    this->text->setPosition(shape_bounds.left + shape_bounds.width/2.0f, shape_bounds.top + shape_bounds.height/2.0f);
}


inline void UIButton::draw(void) const noexcept {
    this->window->draw(*this->shape);

    if (this->text) {
        this->window->draw(*this->text);
    }
}


inline void UIButton::press(void) {
    if (!this->shape) {
        throw std::runtime_error("UIButton::press() -> Nothing to press.");
    }

    if (this->is_hovered) {
        this->shape->setFillColor(*this->press_color);
        this->is_pressed = true;
    }

    if (this->action) {
        this->action();
    }
}


inline void UIButton::hover(void) noexcept {
    if (this->is_mover_over_shape()) {
        if (!this->is_pressed) {
            this->shape->setFillColor(*this->hover_color);
            this->is_hovered = true;
        }
    } else {
        this->shape->setFillColor(*this->base_color);
        this->is_hovered = false;
        this->is_pressed = false;
    }
}


inline void UIButton::release(void) {
    this->shape->setFillColor((this->is_hovered) ? *this->hover_color : *this->base_color);
    this->is_pressed = false;
}


#endif //GRAVITY_UIBUTTON_H
