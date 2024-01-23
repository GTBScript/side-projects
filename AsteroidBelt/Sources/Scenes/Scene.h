#ifndef ASTEROIDBELT_SCENE_H
#define ASTEROIDBELT_SCENE_H
#include "../UIElements/UIButtons/UIButton.h"
#include "../UIElements/UIButtons/DynamicUIButton.h"
#include "../UIElements/UIButtons/StaticUIButton.h"
#include "../Includes.h"
#include "../UIElements/UIImages/UIImage.h"


class Scene {
private:
    unsigned short current_frame = 0;
    const float frame_duration = 0.1f;
    bool music_plays = false;

protected:
    std::list<std::unique_ptr<UIButton>> buttons;
    std::list<std::unique_ptr<UIImage>> images;

    std::vector<Texture> background_gif;
    std::list<Text> labels;

    std::unique_ptr<Music>          background_music = nullptr;
    std::unique_ptr<Texture>        background_image = nullptr;
    std::unique_ptr<SoundBuffer>    sound_buffer = nullptr;
    std::unique_ptr<Sprite>         sprite = nullptr;
    std::unique_ptr<Clock>          scene_clock = nullptr;

    RenderWindow * window = nullptr;

    void run_background();
public:

    void load();
    void exit();
    void hover();
    void press();
    void release();
    void set_background_music(const std::string & path);
    void set_background_image(const std::string & path);
    void set_animated_background(const std::string & path);
    void attach_window(RenderWindow & _window);
    void add_button(std::unique_ptr<UIButton> & button);
    void add_image(std::unique_ptr<UIImage> & texture);


};


#endif //ASTEROIDBELT_SCENE_H
