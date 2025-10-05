#include "removeIdsFromDirs.hxx"

#include "utils/utils.hxx"

#include <vector>
#include <string>

bool RemoveIdsFromDirs::is_done(fs::path entity) const {
    const char separator = ' ';
    std::vector<std::string> words = Utils::split_filename(entity, separator);

    return words.size() == 1;
}

void RemoveIdsFromDirs::execute() {
    std::string eurocode = "";
    std::string info_msg;
    fs::path new_dir;

    Utils::remove_prefix(current_dir, &eurocode, ' ');
    new_dir = ((fs::path)current_dir.string()).replace_filename(eurocode);

    try {
        if (is_done(current_dir)) {
            return;
        }

        info_msg = Utils::handle_info("RENAME FOLDER", initial_dir, current_dir, new_dir, dye::yellow);

        if (!db.contains_eurocode(eurocode) || eurocode == "") {
            throw std::logic_error("First word in folder's name (" + eurocode + ") isn't valid eurocode");
        }

        fs::rename(current_dir, new_dir);
        Utils::handle_success(info_msg);
    } catch (fs::filesystem_error err) {
        Utils::handle_error(info_msg, err);
    } catch (std::logic_error err) {
        Utils::handle_error(info_msg, err, "NAME ERROR", dye::on_yellow);
    }
}
