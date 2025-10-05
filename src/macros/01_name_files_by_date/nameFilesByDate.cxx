#include "nameFilesByDate.hxx"

#include "utils/utils.hxx"

void NameFilesByDate::sort_files_by_update_time(std::vector<fs::path>& files) {
    sort(files.begin(), files.end(), [] (fs::path& e1, fs::path& e2) { return fs::last_write_time(e1) < fs::last_write_time(e2); });
}

bool NameFilesByDate::is_done(fs::path entity) const {
    throw std::logic_error("Is not implemented"); //TODO: create separate exception for this
}

void NameFilesByDate::execute() {
    std::vector<fs::path> files = Utils::get_files(current_dir);
    Utils& utils = Utils::getInstance();
    std::vector<std::string> legit_filenames = utils.get_legit_filenames();
    int file_count = 0;

    sort_files_by_update_time(files);

    for (auto& file : files) {
        try {
            if (file_count >= legit_filenames.size()) {
                throw std::logic_error("Not enough legit filenames");
            }

            fs::path new_file = ((fs::path)file.string()).replace_filename(legit_filenames[file_count] + file.extension().string()); //TODO: use original file ext
            file_count++;

            if (file == new_file) {
                continue;
            }

            Utils::handle_info("RENAME", initial_dir, file, new_file, dye::yellow);

            if (fs::exists(new_file)) {
                throw std::runtime_error("File with such new name already exists");
            }

            fs::rename(file, new_file);
            Utils::handle_success();
        } catch (fs::filesystem_error err) {
           Utils::handle_error(err);
        } catch (std::runtime_error err) {
            Utils::handle_error(err);
        } catch (std::logic_error err) {
            Utils::handle_error(err);
            break;
        }
    }
}
