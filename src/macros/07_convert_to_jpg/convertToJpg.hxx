#pragma once

#include "macros/macros.hxx"

class ConvertToJpg : public Macros {
    private:
        bool is_image(fs::path file);
    public:
        ConvertToJpg(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir) {}
        std::string get_name() const { return "Convert image files to .jpg"; };
        bool is_exec_on_dirs_set() const { return true; };
        bool is_done(fs::path entity) const;
        void execute();
};
