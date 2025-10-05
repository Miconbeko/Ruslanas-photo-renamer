#include "copyFileSetPrefixWithAB.hxx"

#include "utils/utils.hxx"
#include "macros/02_append_EC_to_file/appendECToFile.hxx"
#include "database/database.hxx"

#include <string>

bool CopyFileSetPrefixWithAB::is_done(fs::path entity) const {
    const char separator = '-';

    std::string file_name = entity.filename().string();
    std::string prefix = file_name.substr(0, file_name.find(separator));

    return db.contains_id(prefix);
}

void CopyFileSetPrefixWithAB::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);
    std::string current_dir_name = current_dir.filename().string();
    std::vector<std::string> ids = db.get_ids(current_dir_name);

    AppendECToFile append_eurocode(current_dir.string(), initial_dir.string());

    try {
        if (ids.size() == 0) {
            throw std::logic_error("This Eurocode (" + current_dir_name + ") is invalid or has no identifiers");
        }
    } catch (std::logic_error err) {
        Utils::handle_error("", err, "NAME ERROR", dye::on_yellow);
    }


    for (auto const& file : files) {
        if (is_done(file))
            continue;

        for (const std::string& id : ids) {
            fs::path new_file = Utils::remove_prefix(file);
            new_file = Utils::append_prefix(new_file, id);

            try {
                info_msg = Utils::handle_info("COPY", initial_dir, file, new_file, dye::aqua);

                if (!append_eurocode.is_done(file)) {
                    throw std::runtime_error("This file doesn't have Eurocode as prefix");
                }

                fs::copy(file, new_file);
                Utils::handle_success(info_msg);
            } catch (fs::filesystem_error err) {
                Utils::handle_error(info_msg, err);
            } catch (std::runtime_error err) {
                Utils::handle_error(info_msg, err);
            }
        }
    }
}
