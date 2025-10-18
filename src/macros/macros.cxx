#include "macros.hxx"

void Macros::set_current_dir(fs::path dir) {
    Utils::check_folder(dir);

    this->current_dir = dir;
}

void Macros::set_initial_dir(fs::path dir) {
    Utils::check_folder(dir);

    this->initial_dir = dir;
}

bool Macros::is_done(fs::path entity) const {
    throw std::logic_error("Is not implemented"); //TODO: create separate exception for this
}
