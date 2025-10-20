#pragma once

#include "macros/macros.hxx"

#include "map"

namespace fs = std::filesystem;

class RenameOldECFoldersToNew : public Macros {
    private:
        fs::path dest_dir;
    public:
        RenameOldECFoldersToNew(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, false, true) {}
        std::string get_name() const { return "Copy folders with old EC and rename to new EC"; }
        void pre_execute();
        void execute();
};
