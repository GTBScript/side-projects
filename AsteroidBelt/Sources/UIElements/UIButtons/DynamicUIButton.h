#ifndef ASTEROIDBELT_DYNAMICUIBUTTON_H
#define ASTEROIDBELT_DYNAMICUIBUTTON_H
#include "StaticUIButton.h"

class DynamicUIButton : public StaticUIButton {
private:
    std::unique_ptr<Texture> base_texture = nullptr;
    std::unique_ptr<Texture> hover_texture = nullptr;
    std::unique_ptr<Texture> press_texture = nullptr;

public:
    explicit DynamicUIButton();

    void press(void) noexcept override;
    void hover(void) noexcept override;
    void release(void) noexcept override;
    void set_position(Vector2f position) override;

    void set_base_texture(const std::string & texture_path);
    void set_hover_texture(const std::string & texture_path);
    void set_press_texture(const std::string & texture_path);
};

#endif //ASTEROIDBELT_DYNAMICUIBUTTON_H
