#include "../../Headers/Scene/Scene.h"


void Scene::exit(void) {

}

void Scene::attach_window(RenderWindow * __window) {
    this->window = __window;
}

void Scene::add_label(std::unique_ptr<UILabel> & text) {
    this->labels.emplace_back(std::move(text));
}

void Scene::add_button(std::unique_ptr<UIButton> & button) {
    this->buttons.emplace_back(std::move(button));
}
