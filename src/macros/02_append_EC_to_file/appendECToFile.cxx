#include "appendECToFile.hxx"

#include "utils/utils.hxx"
#include "macros/03_copy_file_set_prefix_with_AB/copyFileSetPrefixWithAB.hxx"
#include "database/database.hxx"

#include <string>
#include <vector>

bool AppendECToFile::is_done(fs::path entity) const {
    const char separator = '-';
    std::string file_name = entity.filename().string();
    std::string current_dir_name = current_dir.filename().string();

    int separator_pos = file_name.find(separator);

    return separator_pos != std::string::npos
            && file_name.substr(0, separator_pos) == current_dir_name;
}

void AppendECToFile::execute() {
    std::vector<fs::path> files = Utils::get_files(current_dir);
    std::string current_dir_name = current_dir.filename().string();
    Utils& utils = Utils::getInstance();

    CopyFileSetPrefixWithAB copy_file_set_prefix_ab(current_dir.string(), initial_dir.string());

    for (auto const& file : files) {
        fs::path new_file = Utils::append_prefix(file, current_dir_name);

        try {
            if (AppendECToFile::is_done(file)) {
                // throw runtime_error("This file is already have prefix in its name");
                continue;
            }
            if (copy_file_set_prefix_ab.is_done(file)) {
                // throw runtime_error("This file was processed with 2-nd macros");
                continue;
            }

            Utils::handle_info("RENAME", initial_dir, file, new_file, dye::yellow);

            if (!utils.is_legit(file, false)) {
                throw std::logic_error("This file has wrong name");
            }
            if (!db.contains_eurocode(current_dir_name)) {
                throw std::logic_error("This Eurocode(`" + current_dir_name + "`) doesn't exist");
            }

            fs::rename(file, new_file);
            Utils::handle_success();
        } catch (fs::filesystem_error err) {
            Utils::handle_error(err);
        } catch (std::runtime_error err) {
            Utils::handle_error(err);
        } catch (std::logic_error err) {
            Utils::handle_error(err, "NAME ERROR", dye::on_yellow);
        }
    }
}
