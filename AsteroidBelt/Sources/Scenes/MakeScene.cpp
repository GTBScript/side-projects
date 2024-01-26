#include "MakeScene.h"
#include "../Actions.h"

std::unique_ptr<Scene> MakeScene::MakeMenu(RenderWindow & window) {
    std::unique_ptr<Scene> menu = std::make_unique<Scene>();
    menu->attach_window(window);
    menu->set_animated_background("../../Pictures/MainMenuBackground/bckg-");

    std::unique_ptr<UIButton> start = std::make_unique<DynamicUIButton>();
    start->assign_window(window);
    start->set_hover_color(Color::Cyan);
    start->set_position({50, 300});
    start->set_size({200, 100});
    DynamicUIButton * s_ptr = dynamic_cast<DynamicUIButton *>(start.get());
    s_ptr->set_texture("../../Pictures/MainMenuUI/Play.png");
    menu->add_button(start);

    std::unique_ptr<UIButton> options = std::make_unique<DynamicUIButton>();
    options->assign_window(window);
    options->set_hover_color(Color::Cyan);
    options->set_position({50, 400});
    options->set_size({300, 100});
    s_ptr = dynamic_cast<DynamicUIButton *>(options.get());
    s_ptr->set_texture("../../Pictures/MainMenuUI/Options.png");
    menu->add_button(options);

    std::unique_ptr<UIButton> exit = std::make_unique<DynamicUIButton>();
    exit->assign_window(window);
    exit->set_hover_color(Color::Cyan);
    exit->set_position({50, 500});
    exit->set_size({200, 100});
    s_ptr = dynamic_cast<DynamicUIButton *>(exit.get());
    s_ptr->set_texture("../../Pictures/MainMenuUI/Exit.png");
    exit->attach_action([&window](){Actions::close(window);});
    menu->add_button(exit);

    std::unique_ptr<UIImage> img1 = std::make_unique<UIImage>();
    img1->set_texture("../../Pictures/Asteroid-Belt.png");
    img1->set_size({650, 100});
    img1->set_position({160, 50});
    img1->assign_window(window);

    menu->add_image(img1);

    return menu;
}

std::unique_ptr<Scene> MakeScene::MakeOptions(RenderWindow & window) {
    std::unique_ptr<Scene> options = std::make_unique<Scene>();
    options->attach_window(window);




    return std::unique_ptr<Scene>();
}




