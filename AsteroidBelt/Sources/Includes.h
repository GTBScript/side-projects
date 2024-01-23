#ifndef ASTEROIDBELT_INCLUDES_H
#define ASTEROIDBELT_INCLUDES_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <stack>
#include <list>

using namespace sf;

namespace Internal {
    constexpr unsigned short __FRAME_RATE__ = 70;
    constexpr unsigned short __WIN_WIDTH__ = 960;
    constexpr unsigned short __WIN_HEIGHT__ = 720;
    extern bool __SOUNDS_MUTED__;
    extern bool __MUSIC_MUTED__;
}

#endif //ASTEROIDBELT_INCLUDES_H
