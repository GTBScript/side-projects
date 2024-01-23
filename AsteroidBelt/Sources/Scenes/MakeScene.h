#ifndef ASTEROIDBELT_MAKESCENE_H
#define ASTEROIDBELT_MAKESCENE_H
#include "../UIElements/UIButtons/UIButton.h"
#include "../UIElements/UIButtons/DynamicUIButton.h"
#include "../UIElements/UIButtons/StaticUIButton.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "../UIElements/UIImages/UIImage.h"

class MakeScene {
public:
    static std::unique_ptr<Scene> MakeMenu(RenderWindow & window);
};


#endif //ASTEROIDBELT_MAKESCENE_H
