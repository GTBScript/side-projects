#include "Actions.h"
#include "Internal.h"

void Actions::pause_play_music(Music * music) noexcept {
    if (music->getStatus() == SoundSource::Paused) {
        music->play();
        Internal::__MUSIC_MUTED__ = false;
    } else {
        music->pause();
        Internal::__MUSIC_MUTED__ = true;
    }
}


void Actions::set_music(std::unique_ptr<Music> & music_ptr, const std::string & path) {
    if (!music_ptr) {
        throw std::runtime_error("The pointer is NULL");
    }

    if (!music_ptr->openFromFile(path)) {
        throw std::runtime_error("Invalid or the music path does not exist.");
    }

    music_ptr->setLoop(true);
    music_ptr->setVolume(10.f);
}


void Actions::none() noexcept {}


void Actions::close(RenderWindow & window) {
    window.close();
}


