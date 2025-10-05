#include "renameABToECFiles.hxx"

#include "utils/utils.hxx"

#include <string>
#include <vector>

bool RenameABToECFiles::is_done(fs::path entity) const {
    std::string prefix = "";
    fs::path suffix = Utils::remove_prefix(entity, &prefix);

    if (prefix == "") {
        prefix = suffix.stem().string();
    }

    return db.contains_eurocode(prefix);
}

void RenameABToECFiles::execute() {
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string prefix = "";
        fs::path suffix = Utils::remove_prefix(file, &prefix);
        std::string new_file_name;
        fs::path new_file;
        bool is_no_suffix = false;

        if (prefix == "") {
            prefix = suffix.stem().string();
            is_no_suffix = true;
        }

        try {
            if (is_done(file)) {
                continue;
            }

            if (!db.contains_id(prefix)) {
                throw std::runtime_error("Prefix (" + prefix + ") is not an AB");
            }

            prefix = db.get_eurocode(prefix);

            if (is_no_suffix) {
                new_file = suffix.replace_filename(prefix + suffix.extension().string());
            } else {
                new_file = Utils::append_prefix(suffix, prefix);
            }

            Utils::handle_info("RENAME", initial_dir, file, new_file, dye::yellow);

            if (fs::exists(new_file)) {
                throw std::logic_error("File with name (" + new_file.filename().string() + ") is already exit");
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
