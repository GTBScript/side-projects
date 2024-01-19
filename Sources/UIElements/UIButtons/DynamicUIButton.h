#ifndef ASTEROIDBELT_DYNAMICUIBUTTON_H
#define ASTEROIDBELT_DYNAMICUIBUTTON_H
#include "UIButton.h"

class DynamicUIButton : public UIButton {
private:
    std::unique_ptr<Texture> base_texture = nullptr;
    std::unique_ptr<Texture> hover_texture = nullptr;
    std::unique_ptr<Texture> press_texture = nullptr;

public:
    explicit DynamicUIButton();

    void press() noexcept override;
    void hover() noexcept override;
    void release() noexcept override;
    void set_position(Vector2f position) override;

    void set_base_texture(const std::string & texture_path);
    void set_hover_texture(const std::string & texture_path);
    void set_press_texture(const std::string & texture_path);
};

#endif //ASTEROIDBELT_DYNAMICUIBUTTON_H
