#include "renameOldECFoldersToNew.hxx"

#include "utils/utils.hxx"

bool contains(std::vector<std::string> vec, std::string find) {
    for (std::string el : vec) {
        if (el == find) {
            return true;
        }
    }
    return false;
}

void RenameOldECFoldersToNew::execute() {
    std::string info_msg;
    std::vector<fs::path> dirs = Utils::get_files(current_dir);

    for (auto const& dir : dirs) {
        std::string old_eurocode = dir.filename().string();

        try {
            if (!db.contains_old_eurocode(old_eurocode)) {
                throw std::runtime_error("The folder name (" + old_eurocode + ") is not an old EC");
            }

            std::vector<std::string> old_ids = db.get_old_eurocode_ids(old_eurocode);

            if (old_ids.size() == 0) {
                throw std::runtime_error("This old EC (" + old_eurocode + ") doesn't have AB");
            }

            std::string eurocode = db.get_eurocode(old_ids.at(0));

            if (eurocode == old_eurocode) {
                continue;
            }

            fs::path new_dir = ((fs::path)dest_dir).append(eurocode);

            info_msg = Utils::handle_info("COPY", "", dir, new_dir, dye::yellow);

            // if (old_ids.size() != new_ids.size()) {
            //     throw std::runtime_error("Old EC (" + old_eurocode + ") has (" + std::to_string(old_ids.size()) + ") connected ABs. One of this ABs, (" + old_ids.at(0) + ") connects to new EC (" + eurocode + ") that has (" + std::to_string(new_ids.size()) + ") connected ABs");
            // }

            // for (std::string old_id : old_ids) {
            //     if (!contains(new_ids, old_id)) {
            //         throw std::runtime_error("AB (" + old_id + ") that connects to old EC (" + old_eurocode + ") doesn't connect to new EC (" + eurocode + ")");
            //     }
            // }
            for (std::string old_id : old_ids) {
                std::string another_eurocode = db.get_eurocode(old_id);

                if (another_eurocode == "") {
                    throw std::runtime_error("AB (" + old_id + ") that connects to old EC (" + old_eurocode + ") does not connect to new EC");
                }
                if (eurocode != another_eurocode) {
                    throw std::runtime_error("AB (" + old_id + ") that connects to old EC (" + old_eurocode + ") also connects to new EC (" + another_eurocode + "). But at least one of other ABs of old EC connects to different new EC (" + eurocode + "). Stop processing. Fix this ambiguity");
                }
            }

            // std::vector<std::string> new_ids = db.get_ids(eurocode);

            // if (new_ids.size() == 0) {
            //     throw std::runtime_error("This new EC (" + eurocode + ") doesn't have AB");
            // }

            // for (std::string new_id : new_ids) {
            //     // if (!contains(old_ids, new_id)) {
            //     //     throw std::runtime_error("AB (" + new_id + ") of new EC (" + eurocode + ") isn't included in ABs of old EC (" + old_eurocode + "). Stop processing. Fix this ambiguity");
            //     // }

            //     std::string another_old_eurocode = db.get_old_eurocode_by_id(new_id);

            //     std::cout << new_id << " " << old_eurocode << " " << another_old_eurocode << std::endl;

            //     if (another_old_eurocode == "") {
            //         throw std::runtime_error("AB (" + new_id + ") does not have old EC");
            //     }
            //     if (old_eurocode != another_old_eurocode) {
            //         throw std::runtime_error("AB (" + new_id + ") that connects to new EC (" + eurocode + ") also connects to old EC (" + another_old_eurocode + ") that isn't equal to folder name (" + old_eurocode + "). Stop processing. Fix this ambiguity");
            //     }
            // }

            fs::copy(dir, new_dir, fs::copy_options::recursive);
            Utils::handle_success(info_msg);
        } catch (std::runtime_error err) {
            Utils::handle_error(info_msg, err);
        }
    }
}

void RenameOldECFoldersToNew::pre_execute() {
    std::wstring dir_wstr;

    while (true) {
        std::cout << "Enter copy destination folder path: \t";
        getline(std::wcin >> std::ws, dir_wstr);
        dest_dir = dir_wstr;

        try {
            if (!fs::is_directory(dest_dir)) {
                throw std::invalid_argument("Invalid folder path");
            }

            return;
        } catch (std::invalid_argument err) {
            Utils::handle_error("", err.what() + std::string(". Try again"));
        }
    }
}
