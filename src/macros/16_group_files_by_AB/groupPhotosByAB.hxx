#pragma once

#include "macros/macros.hxx"

class GroupPhotosByAB : public Macros {
    public:
        GroupPhotosByAB(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Group AB files by AB"; }
        bool is_exec_on_dirs_set() const { return false; };
        bool is_done(fs::path entity) const;
        void execute();
};
