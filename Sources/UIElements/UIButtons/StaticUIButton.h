#ifndef ASTEROIDBELT_STATICUIBUTTON_H
#define ASTEROIDBELT_STATICUIBUTTON_H
#include "UIButton.h"

class StaticUIButton : public UIButton {
private:
    std::unique_ptr<Color> base_color = nullptr;
    std::unique_ptr<Color> press_color = nullptr;
    std::unique_ptr<Text> text = nullptr;
    std::unique_ptr<Font> font = nullptr;

public:
    explicit StaticUIButton();

    void press() noexcept override;
    void hover() noexcept override;
    void release() noexcept override;
    void set_position(Vector2f position) override;

    void set_base_color(Color color);
    void set_press_color(Color color);
    void set_string(const std::string & string) noexcept;
    void set_font(const std::string & font_path);
    void set_font_size(unsigned size) noexcept;
};

#endif //ASTEROIDBELT_STATICUIBUTTON_H
