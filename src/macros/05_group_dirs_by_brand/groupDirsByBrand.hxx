#pragma once

#include "macros/macros.hxx"

class GroupDirsByBrand : public Macros {
    public:
        GroupDirsByBrand(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, false, true) {}
        std::string get_name() const { return "Group folders by Brand"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
