#include "../../Headers/Scene/MakeScene.h"

std::unique_ptr<Scene> MakeScene::MakeMainMenu(RenderWindow * window) {
    std::unique_ptr<Scene> menu = std::make_unique<MainMenu>();
    menu->attach_window(window);
    MainMenu * ptr = dynamic_cast<MainMenu *>(menu.get());
    ptr->set_background_image("../../Assets/Pictures/main_menu_background.png");

    std::unique_ptr<UIButton> start = std::make_unique<UIButton>(window);
    start->set_base_color({250,250,250,180});
    start->set_hover_color({200, 200, 200, 180});
    start->set_press_color({100, 100, 100, 180});
    start->set_size({300, 60});
    start->set_position({static_cast<float>(window->getSize().x / 2) - start->self_shape()->getSize().x / 2, 380});
    start->set_text("PLAY");
    start->self_text()->setLetterSpacing(4);
    start->set_font("../../Assets/Fonts/cambria.ttc");
    start->set_text_color(Color::Black);
    start->set_font_size(30);
    menu->add_button(start);

    std::unique_ptr<UIButton> options = std::make_unique<UIButton>(window);
    options->set_base_color({250, 250, 250, 180});
    options->set_hover_color({200, 200, 200, 180});
    options->set_press_color({100, 100, 100, 180});
    options->set_size({300, 60});
    options->set_position({static_cast<float>(window->getSize().x / 2) - options->self_shape()->getSize().x / 2, 460});
    options->set_text("OPTIONS");
    options->self_text()->setLetterSpacing(4);
    options->set_font("../../Assets/Fonts/cambria.ttc");
    options->set_text_color(Color::Black);
    options->set_font_size(30);
    menu->add_button(options);

    std::unique_ptr<UIButton> exit = std::make_unique<UIButton>(window);
    exit->set_base_color({250, 250, 250, 180});
    exit->set_hover_color({200, 200, 200, 180});
    exit->set_press_color({100, 100, 100, 180});
    exit->set_size({300, 60});
    exit->set_position({static_cast<float>(window->getSize().x / 2) - exit->self_shape()->getSize(). x / 2, 540});
    exit->set_text("EXIT");
    exit->self_text()->setLetterSpacing(4);
    exit->set_font("../../Assets/Fonts/cambria.ttc");
    exit->set_text_color(Color::Black);
    exit->set_font_size(30);
    menu->add_button(exit);

    std::unique_ptr<UILabel> title = std::make_unique<UILabel>(window);
    title->self()->setString("GRAVITY");
    title->self()->setPosition({100, 100});
    title->self()->setLetterSpacing(4);
    title->self()->setFillColor(Color::White);
    title->self()->setOutlineColor(Color::White);
    title->self()->setCharacterSize(60);
    menu->add_label(title);


    return menu;
}
