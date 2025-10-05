#pragma once

#include "macros/macros.hxx"

class GroupDirsByModel : public Macros {
    public:
        GroupDirsByModel(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, false, true) {}
        std::string get_name() const { return "Group folders by Model"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
