#include <SFML/Graphics/RenderWindow.hpp>
#include "../Headers/Scene/Scene.h"
#include "../Headers/Scene/MakeScene.h"
#include <stack>
#include <atomic>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <vld.h>

using namespace sf;

constinit static const unsigned short WIN_WIDTH = 1280;
constinit static const unsigned short WIN_HEIGHT = 720;
constinit static const unsigned short FPS = 100;
static std::stack<std::unique_ptr<Scene>> scene_stack;

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    RenderWindow window (VideoMode(WIN_WIDTH, WIN_HEIGHT), "Gravity", Style::Titlebar | Style::Close);
    window.setFramerateLimit(FPS);
    Image icon;


    if (!icon.loadFromFile("../../Assets/Pictures/icon.jpg")) {
        throw std::runtime_error("main() -> Invalid or the icon doesn't exist.");
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    scene_stack.emplace(MakeScene::MakeMainMenu(&window));


    while (window.isOpen()) {
        Event event {};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    goto end;

                case Event::MouseMoved:
                    scene_stack.top()->hover();
                    break;

                case Event::MouseButtonPressed:
                    scene_stack.top()->press();
                    break;

                case Event::MouseButtonReleased:
                    scene_stack.top()->release();

                default:
                    break;
            }
        }

        window.clear();
        scene_stack.top()->load();
        window.display();
    }

    end:

    if (!window.isOpen()) {
        while (!scene_stack.empty()) {
            scene_stack.pop();
        }

    }

    return EXIT_SUCCESS;
}