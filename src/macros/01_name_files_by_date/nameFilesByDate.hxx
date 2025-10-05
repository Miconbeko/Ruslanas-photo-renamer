#pragma once

#include "macros/macros.hxx"

#include <filesystem>
#include <vector>

class NameFilesByDate : public Macros {
    private:
        void sort_files_by_update_time(std::vector<fs::path>& files);
    public:
        NameFilesByDate(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Name in order of download"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
