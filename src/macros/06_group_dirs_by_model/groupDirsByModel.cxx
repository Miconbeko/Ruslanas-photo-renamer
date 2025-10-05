#include "groupDirsByModel.hxx"

#include "utils/utils.hxx"

#include <vector>
#include <string>

bool GroupDirsByModel::is_done(fs::path entity) const {
    std::string eurocode = entity.filename().string();

    return db.contains_eurocode(eurocode) && entity.parent_path().filename() == eurocode.substr(0,4);
}

void GroupDirsByModel::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string eurocode = file.filename().string();
        std::string model = eurocode.substr(0, 4);
        fs::path new_dir = file.parent_path().append(model);
        fs::path new_file = ((fs::path)new_dir.string()).append(file.filename().string());

        try {
            if (!fs::is_directory(file)) {
                continue;
            }
            if (is_done(file)) {
                // throw runtime_error("This Eurocode folder is already where it should be");
                continue;
            }

            info_msg = Utils::handle_info("GROUP", initial_dir, file, new_file, dye::light_purple);

            if (!db.contains_eurocode(eurocode)) {
                throw std::logic_error("This Eurocode(`" + eurocode + "`) doesn't exist");
            }
            if (fs::exists(new_file)) {
                throw std::runtime_error("This Eurocode folder already exists in the Model folder");
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
