#include "../../Headers/UI/UILabel.h"

std::unique_ptr<Text> & UILabel::self(void) noexcept {
    return this->text;
}
