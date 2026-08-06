#include "groupPhotosByAB.hxx"

#include "utils/utils.hxx"

#include <string>
#include <vector>

bool GroupPhotosByAB::is_done(fs::path entity) const {
    std::string id = "";
    Utils::remove_prefix(entity, &id);

    return id != "" && db.contains_id(id) && entity.parent_path().filename() == id;
}

void GroupPhotosByAB::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string id;
        Utils::remove_prefix(file, &id);

        fs::path new_dir = file.parent_path().append(id);
        fs::path new_file = ((fs::path)new_dir.string()).append(file.filename().string());

        try {
            if (fs::is_directory(file)) {
                continue;
            }
            if (is_done(file)) {
                continue;
            }

            info_msg = Utils::handle_info("GROUP", initial_dir, file, new_file, dye::light_purple);

            if (!db.contains_id(id)) {
                throw std::logic_error("This AB(`" + id + "`) doesn't exist");
            }
            if (fs::exists(new_file)) {
                throw std::runtime_error("This file already exists in the AB folder");
            }

            fs::create_directory(new_dir);
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
