#pragma once

#include "macros/macros.hxx"

#include <filesystem>

class CopyTests : public Macros {
    private:
        fs::path copy_dest;
    public:
        CopyTests(std::string current_dir = "", std::string initial_dir = "");
        std::string get_name() const { return "Copy all etalon test cases"; }
        void execute();
};
