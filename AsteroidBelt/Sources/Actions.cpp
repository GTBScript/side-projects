#include "Actions.h"

void Actions::pause_play_music(Music * music) noexcept {
    if (music->getStatus() == SoundSource::Paused) {
        music->play();
        Internal::__MUSIC_MUTED__ = false;
    } else {
        music->pause();
        Internal::__MUSIC_MUTED__ = true;
    }
}

void Actions::play_sound(Sound * sound) noexcept {
    if (!Internal::__SOUNDS_MUTED__) {
        sound->play();
    }
}

void Actions::none() noexcept {}

void Actions::close(RenderWindow * window) {
    window->close();
}

void Actions::load_main_menu_logo(Texture & texture) {
    if (!texture.loadFromFile("../Pictures/Asteroid-Belt.png")) {
        throw std::runtime_error("Error loading image.");
    }
}

void Actions::load_main_menu_music(Music * music) {
    if (!music->openFromFile("../Audio/Music/MainTheme.mp3")) {
        throw std::runtime_error("Couldn't open the music file.");
    }
}

std::vector<Texture> Actions::load_main_menu_background(sf::Texture &texture, sf::RenderWindow &window, sf::Sprite &sprite) {
    std::vector<Texture> textures;

    for (unsigned short i=0; i<40; i++) {
        if (!texture.loadFromFile("../Pictures/MainMenuBackground/bckg-"+std::to_string(i)+".jpg")) {
            throw std::runtime_error("Failed to load frame " + std::to_string(i));
        } else {
            textures.emplace_back(std::move(texture));
        }

    }

    if (!textures.empty()) {
        sprite.setTexture(textures[0]);
        const Vector2u textureSize = textures[0].getSize();
        const Vector2u windowSize = window.getSize();
        const float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
        const float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
        sprite.setScale(scaleX, scaleY);
    }

    return textures;
}

void Actions::disable_sounds() noexcept {
    Internal::__SOUNDS_MUTED__ = !(Internal::__SOUNDS_MUTED__);
}

