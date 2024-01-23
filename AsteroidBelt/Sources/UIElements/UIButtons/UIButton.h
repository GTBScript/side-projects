#ifndef ASTEROIDBELT_UIBUTTON_H
#define ASTEROIDBELT_UIBUTTON_H
#include "../../Includes.h"

class UIButton {
protected:
    std::unique_ptr<RectangleShape> shape       = nullptr;
    std::unique_ptr<Sound> sound                = nullptr;
    std::unique_ptr<SoundBuffer> sound_buffer   = nullptr;
    std::unique_ptr<Color> hover_color          = nullptr;
    std::unique_ptr<Texture> texture            = nullptr;
    RenderWindow * window                       = nullptr;

    bool is_mouse_over_shape(void) const;
    bool pressed = false;
    bool hovered = false;

    std::function<void()> action;
    void execute();

public:
    UIButton();
    ~UIButton();

    virtual void press(void) = 0;
    virtual void hover(void) = 0;
    virtual void release(void) = 0;
    virtual void set_position(Vector2f position);

    template <typename Func> void attach_action(Func f);
    void assign_window(RenderWindow & _window);
    void assign_sound(const std::string & sound_path);
    void set_hover_color(Color color);
    void set_size(Vector2f size);
    void draw(void);
    void set_texture(const std::string & texture_path);

};

template <typename Func> void UIButton::attach_action(Func f) {
    this->action = f;
}

#endif //ASTEROIDBELT_UIBUTTON_H
