#pragma once

#include "macros/macros.hxx"
#include "utils/utils.hxx"

#include <filesystem>
#include <vector>
#include <map>

namespace fs = std::filesystem;

class MacrosExecutor {
    private:
        const int COUNT_FROM_ZERO = 0; // TODO: change to BOOL

        int i_macros;
        fs::path initial_dir;
        bool do_execute;

        std::vector<Macros*> macros;
        std::set<int> separators;
        Macros* current_macros;

        void draw_separator();
        bool ask_macros(bool loop_question = true);
        bool ask_path(bool loop_question = true);
        bool ask_exec_on_dirs(bool loop_question = true);
        void ask_all();

        void reset();

        void exec_recursive(fs::path current_dir);
        void exec(fs::path current_dir);
    public:
        MacrosExecutor() { reset(); }
        void add_macros(Macros* macros) { this->macros.push_back(macros); }
        void add_separator(int index) { this->separators.insert(index); }
        void eventLoop();
};
