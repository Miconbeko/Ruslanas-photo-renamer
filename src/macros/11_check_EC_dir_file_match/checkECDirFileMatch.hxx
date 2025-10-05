#pragma once

#include "macros/macros.hxx"

class CheckECDirFileMatch : public Macros {
    public:
        CheckECDirFileMatch(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, true, false) {}
        std::string get_name() const { return "Check if file prefixes and folder names EC match"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
