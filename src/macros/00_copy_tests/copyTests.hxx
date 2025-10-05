#pragma once

#include "macros/macros.hxx"

#include <filesystem>

class CopyTests : public Macros {
    private:
        fs::path copy_dest;
    public:
        CopyTests(std::string current_dir = "", std::string initial_dir = "");
        std::string get_name() const { return "Copy all etalon test cases"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
