#pragma once

#include "macros/macros.hxx"

#include <set>
#include <vector>

class DeleteFilesWithEC : public Macros {
    private:
        std::set<std::string> get_filenames(std::vector<fs::path> files);
        bool is_ready(fs::path file, std::set<std::string> filenames);
    public:
        DeleteFilesWithEC(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Delete files with EC"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
