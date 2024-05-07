#ifndef GRAVITY_SCENE_H
#define GRAVITY_SCENE_H
#include "SFML/Window.hpp"
#include <list>
#include "../UI/UIButton.h"
#include "../UI/UILabel.h"


class Scene {
protected:
    std::list<std::unique_ptr<UIButton>> buttons;
    std::list<std::unique_ptr<UILabel>> labels;

    std::unique_ptr<Clock> clock = nullptr;

    RenderWindow * window = nullptr;

private:


public:
    virtual inline void load(void) noexcept;
    virtual void exit(void);
    virtual inline void hover(void) noexcept;
    virtual inline void press(void) noexcept;
    virtual inline void release(void) noexcept;

    void attach_window(RenderWindow * __window);
    void add_button(std::unique_ptr<UIButton> & button);
    void add_label(std::unique_ptr<UILabel> & text);
};


inline void Scene::load(void) noexcept {
    for (auto & button : this->buttons) {
        button->draw();
    }

    for (auto & label : this->labels) {
        label->draw();
    }
}


inline void Scene::hover(void) noexcept {
    for (auto & button : this->buttons) {
        button->hover();
    }
}


inline void Scene::press(void) noexcept {
    for (auto & button : this->buttons) {
        button->press();
    }
}


inline void Scene::release(void) noexcept{
    for (auto & button : this->buttons) {
        button->release();
    }
}


#endif //GRAVITY_SCENE_H
