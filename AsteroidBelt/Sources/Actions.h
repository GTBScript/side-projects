#ifndef ASTEROIDBELT_ACTIONS_H
#define ASTEROIDBELT_ACTIONS_H
#include "Includes.h"

namespace Actions {
    extern void pause_play_music(Music * music) noexcept;
    extern void modify_music_volume(float volume);
    extern void modify_sound_volume(float volume);
    extern void disable_sounds() noexcept;
    extern void play_sound(Sound * sound) noexcept;
    extern void set_music(std::unique_ptr<Music> & music_ptr, const std::string & path);
    extern void load_options();
    extern void none() noexcept;
    extern void close(RenderWindow & window);
}


#endif //ASTEROIDBELT_ACTIONS_H
