#pragma once

#include "macros/macros.hxx"

class CopyFileSetPrefixWithAB : public Macros {
    public:
        CopyFileSetPrefixWithAB(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Copy files and rename to AB"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
