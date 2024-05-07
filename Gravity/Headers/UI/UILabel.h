#ifndef GRAVITY_UILABEL_H
#define GRAVITY_UILABEL_H
#include <memory>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using namespace sf;

class UILabel {
private:
    std::unique_ptr<Text> text = nullptr;
    RenderWindow * window = nullptr;
public:
    explicit UILabel(RenderWindow * __window) {
        this->window = __window;
        this->text = std::make_unique<Text>();
    }

    inline void draw(void) const noexcept;

    std::unique_ptr<Text> & self(void) noexcept;
};


inline void UILabel::draw(void) const noexcept {
    this->window->draw(*this->text);
}


#endif //GRAVITY_UILABEL_H
