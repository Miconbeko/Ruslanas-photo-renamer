#pragma once

#include "macros/macros.hxx"

#include "map"

namespace fs = std::filesystem;

class CorrelateABWithOldEC : public Macros {
    private:
        std::map<int, std::string> row_to_old_eurocodes;
    public:
        CorrelateABWithOldEC(std::string current_dir = "", std::string initial_dir = "") : Macros::Macros(current_dir, initial_dir, false, true, false, false) {}
        std::string get_name() const { return "Correlate AB with old Eurocode and write it to Database"; }
        void pre_execute();
        void execute();
        void post_execute(bool is_force_stop);
};
