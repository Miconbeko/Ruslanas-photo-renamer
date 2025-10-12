#pragma once

#include "macros/macros.hxx"

namespace fs = std::filesystem;

class CopyMissingABFiles : public Macros {
    private:
        fs::path dest_dir;
    public:
        CopyMissingABFiles(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, true, false) {}
        std::string get_name() const { return "Copy files with missing AB into separate folder"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void pre_execute();
        void execute();
};
