#ifndef GRAVITY_MAKESCENE_H
#define GRAVITY_MAKESCENE_H
#include "Scene.h"
#include "MainMenu.h"


class MakeScene {
public:
    static std::unique_ptr<Scene> MakeMainMenu(RenderWindow * window);
};

#endif //GRAVITY_MAKESCENE_H
