#pragma once

#include "macros/macros.hxx"

class GroupABDirsByBrand : public Macros {
    public:
        GroupABDirsByBrand(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, false, true) {}
        std::string get_name() const { return "Group AB folders by Brand"; }
        bool is_done(fs::path entity) const;
        void execute();
};
