#include "correlateABWithOldEC.hxx"

#include "utils/utils.hxx"

#include <set>

bool CorrelateABWithOldEC::is_done(fs::path entity) const {
    throw std::logic_error("Is not implemented"); //TODO: create separate exception for this
}

void CorrelateABWithOldEC::execute() {
    std::set<std::string> all_ids = db.get_all_ids();

    for (std::string id : all_ids) {
        std::string new_eurocode = db.get_eurocode(id);
        std::string old_eurocode = db.get_old_eurocode_by_id(id);

        if (new_eurocode == "") {
            Utils::handle_error("", "AB (" + id + ") does not have new Eurocode");
        }

        if (new_eurocode == old_eurocode) {
            old_eurocode = "-";
        }

        int row = db.get_row_by_id(id);
        row_to_old_eurocodes.insert(std::make_pair(row, old_eurocode));
    }
}

void CorrelateABWithOldEC::pre_execute() {
    row_to_old_eurocodes.clear();
}

void CorrelateABWithOldEC::post_execute(bool is_force_stop) {
    if (is_force_stop) {
        std::cout << "\nThe execution is stopped forcefully. Result is not written to Database\n";
        return;
    }

    std::cout << "\nWriting result to Database. Please wait\n";

    db.write_old_eurocodes(row_to_old_eurocodes);
}
