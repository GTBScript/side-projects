#include "Scene.h"


void Scene::set_background_music(const std::string & path) {
    if (!this->background_music) {
        this->background_music = std::make_unique<Music>();
    }

    if (!this->background_music->openFromFile(path)) {
        throw std::runtime_error("Invalid or the music path does not exist.");
    }

    this->background_music->setLoop(true);
    this->background_music->play();
}


void Scene::set_background_image(const std::string & path) {
    if (!this->background_image) {
        this->background_image = std::make_unique<Texture>();
    }

    if (!this->background_image->loadFromFile(path)) {
        throw std::runtime_error("Invalid or the image does not exist.");
    }

    if (!this->sprite) {
        this->sprite = std::make_unique<Sprite>();
    }

    this->sprite->setTexture(*this->background_image);
}


void Scene::set_animated_background(const std::string & path) {
    this->background_gif.reserve(40);

    for (unsigned short i=0; i<40; i++) {
        Texture texture__;

        if (!texture__.loadFromFile(path + std::to_string(i)+".png")) {
            throw std::runtime_error("Error retrieving image " + std::to_string(i)+".png");
        }

        this->background_gif.emplace_back(texture__);
    }

    if (!this->sprite) {
        this->sprite = std::make_unique<Sprite>();
    }

    if (!this->background_gif.empty()) {
        this->sprite->setTexture(this->background_gif[0]);
    }
}


void Scene::run_background() {
    if (!this->scene_clock) {
        this->scene_clock = std::make_unique<Clock>();
    }

    if (this->scene_clock->getElapsedTime().asSeconds() > this->frame_duration) {
        this->current_frame = (this->current_frame + 1) % this->background_gif.size();
        this->sprite->setTexture(this->background_gif[this->current_frame]);
        this->scene_clock->restart();
    }
}


void Scene::attach_window(RenderWindow & _window) {
    this->window = &_window;
}


void Scene::load() {

}


void Scene::exit() {

}
