#ifndef ASTEROIDBELT_ACTIONS_H
#define ASTEROIDBELT_ACTIONS_H
#include "Includes.h"

namespace Actions {
    extern void pause_play_music(Music * music) noexcept;
    extern void disable_sounds() noexcept;
    extern void play_sound(Sound * sound) noexcept;
    extern void none() noexcept;
    extern void close(RenderWindow * window);
    extern void load_main_menu_logo(Texture & texture);
    extern void load_main_menu_music(Music * music);
    extern void load_options();
    extern void close_options();
    extern std::vector<Texture> load_main_menu_background(Texture & texture, RenderWindow & window, Sprite & sprite);
}


#endif //ASTEROIDBELT_ACTIONS_H
