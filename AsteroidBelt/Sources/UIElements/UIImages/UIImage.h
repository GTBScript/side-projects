#ifndef ASTEROIDBELT_UIIMAGE_H
#define ASTEROIDBELT_UIIMAGE_H
#include "../../Includes.h"

class UIImage {
private:
    std::unique_ptr<RectangleShape> shape = nullptr;
    std::unique_ptr<Texture> texture      = nullptr;
    RenderWindow * window                 = nullptr;

public:
    explicit UIImage();

    void set_position(Vector2f pos);
    void set_size(Vector2f size);
    void set_texture(const std::string & path);
    Texture & get_texture(void);
    void assign_window(RenderWindow & window_);
    void draw();
};


#endif //ASTEROIDBELT_UIIMAGE_H
