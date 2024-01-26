#ifndef ASTEROIDBELT_INTERNAL_H
#define ASTEROIDBELT_INTERNAL_H
#include "Includes.h"
#include "Scenes/Scene.h"

namespace Internal {
    constexpr unsigned short __FRAME_RATE__ = 70;
    constexpr unsigned short __WIN_WIDTH__ = 960;
    constexpr unsigned short __WIN_HEIGHT__ = 720;
    extern bool __SOUNDS_MUTED__;
    extern bool __MUSIC_MUTED__;
    extern std::stack<std::unique_ptr<Scene>> __SCENE_FOCUS__;
}



#endif //ASTEROIDBELT_INTERNAL_H
