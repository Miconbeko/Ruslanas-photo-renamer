#pragma once

#include "macros/macros.hxx"

#include <string>

class AppendECToFile : public Macros {
    public:
        AppendECToFile(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Add EC to filenames"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
