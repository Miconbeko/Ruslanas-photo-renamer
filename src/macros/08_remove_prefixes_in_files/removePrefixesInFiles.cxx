#include "removePrefixesInFiles.hxx"

#include "utils/utils.hxx"

#include <vector>
#include <string>

bool RemovePrefixesInFiles::is_done(fs::path entity) const {
	throw std::logic_error("Is not implemented"); //TODO: create separate exception for this
}

void RemovePrefixesInFiles::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        fs::path new_file = Utils::remove_prefix(file);

        try {
            if (file.string() == new_file.string()) {
                continue;
            }

            info_msg = Utils::handle_info("RENAME", initial_dir, file, new_file, dye::light_purple);

            if (fs::exists(new_file)) {
                throw std::runtime_error("File with such name is already exists");
            }

            fs::rename(file, new_file);

            Utils::handle_success(info_msg);
        } catch (fs::filesystem_error err) {
            Utils::handle_error(info_msg, err);
        } catch (std::runtime_error err) {
            Utils::handle_error(info_msg, err);
        } catch (std::logic_error err) {
            Utils::handle_error(info_msg, err, "NAME ERROR", dye::on_yellow);
        }
    }
}