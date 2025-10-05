#include "checkECDirFileMatch.hxx"

#include "utils/utils.hxx"

#include <vector>
#include <string>

bool CheckECDirFileMatch::is_done(fs::path entity) const {
	return true;
}

void CheckECDirFileMatch::execute() {
    std::vector<fs::path> files = Utils::get_files(current_dir);
    std::string dir_name = current_dir.filename().string();

	if (!db.contains_eurocode(dir_name)) {
		Utils::handle_error("Folder's name (" + dir_name + ") is not an Eurocode. Skipping check of all files in it");
		return;
	}

    for (auto const& file : files) {
        std::string prefix = "";

        Utils::remove_prefix(file, &prefix);

        try {
            if (prefix == "") {
                Utils::handle_info("CHECK", initial_dir, file, file, dye::light_purple);
                throw std::runtime_error("File has no prefix");
            }
            if (!db.contains_eurocode(prefix)) {
                Utils::handle_info("CHECK", initial_dir, file, file, dye::light_purple);
                throw std::runtime_error("File's prefix (" + prefix + ") is not an Eurocode");
            }
            if (prefix != dir_name) {
                Utils::handle_info("CHECK", initial_dir, file, file, dye::light_purple);
                throw std::runtime_error("File's prefix (" + prefix + ") doesn't match folder name (" + dir_name + ")");
            }
        } catch (fs::filesystem_error err) {
            Utils::handle_error(err);
        } catch (std::runtime_error err) {
            Utils::handle_error(err);
        } catch (std::logic_error err) {
            Utils::handle_error(err, "NAME ERROR", dye::on_yellow);
        }
    }
}