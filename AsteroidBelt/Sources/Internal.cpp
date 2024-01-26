#include "Internal.h"

namespace Internal {
    std::stack<std::unique_ptr<Scene>> __SCENE_FOCUS__;
    bool __MUSIC_MUTED__ = false;
    bool __SOUND_MUTED__ = false;
}
