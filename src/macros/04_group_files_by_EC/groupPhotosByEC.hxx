#pragma once

#include "macros/macros.hxx"

class GroupPhotosByEC : public Macros {
    public:
        GroupPhotosByEC(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Group EC files by EC"; };
        bool is_exec_on_dirs_set() const { return false; };
        bool is_done(fs::path entity) const;
        void execute();
};
