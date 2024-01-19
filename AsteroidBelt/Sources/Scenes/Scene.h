#ifndef ASTEROIDBELT_SCENE_H
#define ASTEROIDBELT_SCENE_H
#include "../UIElements/UIButtons/UIButton.h"
#include "../UIElements/UIButtons/DynamicUIButton.h"
#include "../UIElements/UIButtons/StaticUIButton.h"
#include "../Includes.h"


class Scene {
private:
    unsigned short current_frame = 0;
    const float frame_duration = 0.1f;

protected:
    std::vector<UIButton *> buttons;
    std::vector<Texture> background_gif;
    std::vector<Text> labels;

    std::unique_ptr<Music> background_music = nullptr;
    std::unique_ptr<Texture> background_image = nullptr;
    std::unique_ptr<SoundBuffer> sound_buffer = nullptr;
    std::unique_ptr<Sprite> sprite = nullptr;
    std::unique_ptr<Clock> scene_clock = nullptr;

    RenderWindow * window = nullptr;


public:

    void load();
    void exit();
    void set_background_music(const std::string & path);
    void set_background_image(const std::string & path);
    void set_animated_background(const std::string & path);
    void attach_window(RenderWindow & _window);
    void run_background();
};


#endif //ASTEROIDBELT_SCENE_H
