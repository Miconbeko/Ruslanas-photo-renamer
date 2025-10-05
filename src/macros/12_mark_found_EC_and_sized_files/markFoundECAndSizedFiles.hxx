#pragma once

#include "macros/macros.hxx"

class MarkFoundECAndSizedFiles : public Macros {
    private:
        std::set<std::string> found_eurocodes;
        std::set<std::string> sized_eurocodes;
    public:
        MarkFoundECAndSizedFiles(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, true, false) {}
        std::string get_name() const { return "Mark found EC and files with correct size in Database"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void pre_execute();
        void execute();
        void post_execute(bool is_force_stop);
};
