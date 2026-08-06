#include "groupABDirsByBrand.hxx"

#include "utils/utils.hxx"

#include <vector>
#include <string>

bool GroupABDirsByBrand::is_done(fs::path entity) const {
    std::string id = entity.filename().string();

    if (!db.contains_id(id)) {
        return false;
    }

    std::string eurocode = db.get_eurocode(id);

    return db.contains_eurocode(eurocode) && entity.parent_path().filename() == db.get_brand(eurocode);
}

void GroupABDirsByBrand::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string id = file.filename().string();
        std::string eurocode = db.contains_id(id) ? db.get_eurocode(id) : "";
        std::string brand = db.get_brand(eurocode);
        fs::path new_dir = file.parent_path().append(brand);
        fs::path new_file = ((fs::path)new_dir.string()).append(file.filename().string());

        try {
            if (!fs::is_directory(file)) {
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
                throw std::runtime_error("This AB folder already exists in the Brand folder");
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
