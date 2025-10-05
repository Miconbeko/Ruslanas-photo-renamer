#pragma once

#include "macros/macros.hxx"

class RenameABToECFiles : public Macros {
    public:
        RenameABToECFiles(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Rename AB to EC"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
