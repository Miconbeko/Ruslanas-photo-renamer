#include "deleteFilesWithEC.hxx"

#include "utils/utils.hxx"

#include <string>
#include <iostream>
#include <conio.h>

std::set<std::string> DeleteFilesWithEC::get_filenames(std::vector<fs::path> files) {
    std::set<std::string> filenames;

    for (const auto& file : files) {
        filenames.insert(file.filename().string());
    }

    return filenames;
}

bool DeleteFilesWithEC::is_ready(fs::path file, std::set<std::string> filenames) {
    std::string prefix = "";
    fs::path file_no_prefix = Utils::remove_prefix(file, &prefix);
    std::vector<std::string> ids = db.get_ids(prefix);

    for (auto const& id : ids) {
        std::string filename_to_find = Utils::append_prefix(file_no_prefix, id).filename().string();

        if (filenames.count(filename_to_find) == 0) {
            return false;
        }
    }

    return true;
}

bool DeleteFilesWithEC::is_done(fs::path entity) const {
    const char separator = '-';
    std::string filename = entity.filename().string();
    std::string prefix = filename.substr(0, filename.find(separator));

    return !this->db.contains_id(prefix);
}

void DeleteFilesWithEC::execute() {
    std::vector<fs::path> files = Utils::get_files(current_dir);
    std::set<std::string> filenames = get_filenames(files);

    for (const auto& file : files) {
        std::string prefix = "";
        Utils::remove_prefix(file, &prefix);

        if (!db.contains_eurocode(prefix) || prefix == "") {
            continue;
        }

        try {
            Utils::handle_info("DELETE", initial_dir, file, file, dye::blue);

            if (!is_ready(file, filenames)) {
                throw std::runtime_error("This file has no copies or the error while copying was occurred. Stop deletion");
            }

            fs::remove(file);
            Utils::handle_success();
        } catch (fs::filesystem_error err) {
            Utils::handle_error(err);
        } catch (std::runtime_error err) {
            Utils::handle_error(err);
        }
    }
}
